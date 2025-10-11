#include "csvReader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// The CSVReader handle, keeps track of the file and line buffer.
struct CSVReader {
  FILE *fp;
  char *line;
  size_t linecap;
};

// --- Private Helper Prototypes ---
static ssize_t my_getline(char **lineptr, size_t *n, FILE *stream);
static char *strndup_local(const char *s, size_t n);

// --- Public API ---

/**
 * @brief Opens a CSV file for reading.
 * @param path The path to the CSV file.
 * @return A pointer to a CSVReader handle, or NULL on failure.
 */
CSVReader *csv_open(const char *path) {
  CSVReader *r = malloc(sizeof(*r));
  if (!r)
    return NULL;
  r->fp = fopen(path, "r");
  if (!r->fp) {
    free(r);
    return NULL;
  }
  r->line = NULL;
  r->linecap = 0;
  return r;
}

/**
 * @brief Closes the CSV file and frees all associated resources.
 * @param r The CSVReader handle.
 */
void csv_close(CSVReader *r) {
  if (!r)
    return;
  if (r->fp)
    fclose(r->fp);
  free(r->line);
  free(r);
}

/**
 * @brief Frees the memory for a row returned by csv_read_row.
 * @param row The array of strings (the row).
 * @param count The number of fields in the row.
 */
void csv_free_row(char **row, size_t count) {
  if (!row)
    return;
  for (size_t i = 0; i < count; ++i) {
    free(row[i]);
  }
  free(row);
}

/**
 * @brief Reads a single row from the CSV file.
 *
 * This function handles standard CSV quoting, including escaped quotes ("").
 * The returned row must be freed using csv_free_row().
 *
 * @param r The CSVReader handle.
 * @param out_count A pointer to store the number of fields read.
 * @return An array of strings representing the row, or NULL at end-of-file or
 * on error.
 */
char **csv_read_row(CSVReader *r, size_t *out_count) {
  if (!r || !r->fp)
    return NULL;

  // 1. Read a line from the file.
  ssize_t linelen = my_getline(&r->line, &r->linecap, r->fp);
  if (linelen <= 0) { // EOF or error
    return NULL;
  }
  // Trim trailing newline characters
  while (linelen > 0 &&
         (r->line[linelen - 1] == '\n' || r->line[linelen - 1] == '\r')) {
    r->line[--linelen] = '\0';
  }

  char **fields = NULL;
  size_t fields_cap = 0;
  size_t fields_count = 0;
  char *p = r->line;
  char *end = r->line + linelen;

  // 2. Parse the line field by field.
  while (p < end) {
    char *field = NULL;

    if (*p == '"') {                   // This is a quoted field
      p++;                             // Skip the opening quote
      char *dst = malloc(linelen + 1); // Temp buffer for the unescaped field
      if (!dst)
        goto fail;

      size_t dsti = 0;
      while (p < end) {
        if (*p == '"') { // A quote character
          p++;           // Move past the quote
          // If it's an escaped quote (""), add a single quote to our field
          if (p < end && *p == '"') {
            dst[dsti++] = '"';
            p++;
          } else {
            // Otherwise, it's the end of the field
            break;
          }
        } else {
          dst[dsti++] = *p++;
        }
      }
      dst[dsti] = '\0';
      field = dst;
      // Skip until the next semicolon
      while (p < end && *p != ';')
        p++;

    } else { // This is an unquoted field
      char *sep = memchr(p, ';', end - p);
      if (sep) {
        field = strndup_local(p, (size_t)(sep - p));
        p = sep;
      } else {
        field = strndup_local(p, (size_t)(end - p));
        p = end;
      }
    }

    if (*p == ';')
      p++; // Skip the separator for the next loop iteration

    // 3. Add the parsed field to our dynamic array of fields.
    if (fields_count + 1 > fields_cap) {
      size_t newcap = fields_cap == 0 ? 8 : fields_cap * 2;
      char **tmp = realloc(fields, newcap * sizeof(char *));
      if (!tmp) {
        free(field);
        goto fail;
      }
      fields = tmp;
      fields_cap = newcap;
    }
    fields[fields_count++] = field;
  }

  *out_count = fields_count;
  return fields;

// A common C pattern for centralized cleanup on error.
fail:
  if (fields) {
    for (size_t i = 0; i < fields_count; ++i)
      free(fields[i]);
    free(fields);
  }
  return NULL;
}

// --- Private Helper Implementations ---

/**
 * @brief A portable implementation of getline() that reads a line of any
 * length.
 */
static ssize_t my_getline(char **lineptr, size_t *n, FILE *stream) {
  if (!lineptr || !n || !stream)
    return -1;

  if (*lineptr == NULL) {
    *n = 128; // Initial buffer size
    if ((*lineptr = malloc(*n)) == NULL)
      return -1;
  }

  size_t pos = 0;
  int c;
  while ((c = fgetc(stream)) != EOF) {
    if (pos + 1 >= *n) { // Grow buffer if needed
      size_t new_size = *n * 2;
      char *new_ptr = realloc(*lineptr, new_size);
      if (!new_ptr)
        return -1;
      *lineptr = new_ptr;
      *n = new_size;
    }

    (*lineptr)[pos++] = (char)c;
    if (c == '\n')
      break;
  }

  if (pos == 0 && c == EOF)
    return -1; // No characters read

  (*lineptr)[pos] = '\0';
  return (ssize_t)pos;
}

/**
 * @brief A local, portable version of strndup().
 */
static char *strndup_local(const char *s, size_t n) {
  char *p = malloc(n + 1);
  if (!p)
    return NULL;
  memcpy(p, s, n);
  p[n] = '\0';
  return p;
}

Category **ReadCategories() {
  CSVReader *reader = csv_open("./data/categorias.csv");

  Category **categories = malloc(sizeof(Category) * CATEGORY_MAX);
  if (categories == NULL) {
    return NULL;
  }

  int count = 0;
  while (1) {
    // Read csv row.
    size_t out = 0;
    char **results = csv_read_row(reader, &out);
    if (out == 0) {
      printf("Didn't read any data! Reached the end!");
      break;
    }

    // Create category
    int id = atoi(results[0]);
    char *name = results[1];
    char *filepath = results[2];

    Category *cat = CreateCategory(id, name, filepath);

    // Add category to array of categories
    categories[count] = cat;
    count++;

    // DEBUG:
    // printCategory(categories[count - 1]);
  }

  csv_close(reader);
  return categories;
};

// Book *readBooksFromCategory();
