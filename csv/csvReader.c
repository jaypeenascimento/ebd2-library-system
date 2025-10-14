#include "csvReader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct CSVReader {
  FILE *fp;
  char *line;
  size_t linecap;
};

static ssize_t my_getline(char **lineptr, size_t *n, FILE *stream);
static char *strndup_local(const char *s, size_t n);

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

void csv_close(CSVReader *r) {
  if (!r)
    return;
  if (r->fp)
    fclose(r->fp);
  free(r->line);
  free(r);
}

void csv_free_row(char **row, size_t count) {
  if (!row)
    return;
  for (size_t i = 0; i < count; ++i) {
    free(row[i]);
  }
  free(row);
}

char **csv_read_row(CSVReader *r, size_t *out_count) {
  if (!r || !r->fp)
    return NULL;

  ssize_t linelen = my_getline(&r->line, &r->linecap, r->fp);
  if (linelen <= 0) { // EOF or error
    return NULL;
  }

  // Remove newlines
  while (linelen > 0 &&
         (r->line[linelen - 1] == '\n' || r->line[linelen - 1] == '\r')) {
    r->line[--linelen] = '\0';
  }

  char **fields = NULL;
  size_t fields_cap = 0;
  size_t fields_count = 0;
  char *p = r->line;
  char *end = r->line + linelen;

  // Parse lines
  while (p < end) {
    char *field = NULL;

    if (*p == '"') {
      p++;
      char *dst = malloc(linelen + 1);
      if (!dst)
        goto fail;

      size_t dsti = 0;
      while (p < end) {
        if (*p == '"') {
          p++;
          if (p < end && *p == '"') {
            dst[dsti++] = '"';
            p++;
          } else {
            break;
          }
        } else {
          dst[dsti++] = *p++;
        }
      }
      dst[dsti] = '\0';
      field = dst;
      while (p < end && *p != ';')
        p++;

    } else {
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
      p++;

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

fail:
  if (fields) {
    for (size_t i = 0; i < fields_count; ++i)
      free(fields[i]);
    free(fields);
  }
  return NULL;
}

static ssize_t my_getline(char **lineptr, size_t *n, FILE *stream) {
  if (!lineptr || !n || !stream)
    return -1;

  if (*lineptr == NULL) {
    *n = 128;
    if ((*lineptr = malloc(*n)) == NULL)
      return -1;
  }

  size_t pos = 0;
  int c;
  while ((c = fgetc(stream)) != EOF) {
    if (pos + 1 >= *n) {
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
    return -1;

  (*lineptr)[pos] = '\0';
  return (ssize_t)pos;
}

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
  }

  csv_close(reader);
  return categories;
};

Book **ReadBooksFromCategory(char *filename) {
  if (filename == NULL) {
    printf("Failed to read books from category: Path provided is NULL");
  }

  char path[128] = "";
  sprintf(path, "./data/books/%s", filename);

  CSVReader *reader = csv_open(path);
  if (reader == NULL) {
    printf("\nReader is NULL!\n");
  }

  Book **books = malloc(sizeof(Book) * BOOKS_MAX);
  if (books == NULL) {
    return NULL;
  }

  // Discard first CSV line
  size_t tout = 0;
  csv_read_row(reader, &tout);

  int count = 0;
  while (1) {
    // Read csv row.
    size_t out = 0;
    char **results = csv_read_row(reader, &out);
    if (out == 0) {
      break;
    }

    // Create Book
    int isbn = atoi(results[0]);
    char *titulo = results[1];
    char *autor = results[2];
    int ano = atoi(results[3]);
    int preco = atoi(results[4]);
    int estoque = atoi(results[5]);
    int vendas = atoi(results[6]);

    Book *book = CreateBook(isbn, titulo, autor, ano, preco, estoque, vendas);

    // Add books to array of books
    // Ignore first row since it's not a book data.
    books[count] = book;
    count++;
  }
  books[count + 1] = NULL;

  csv_close(reader);
  return books;
};
