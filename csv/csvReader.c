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

CSVReader *csv_open(const char *path) {
    CSVReader *r = malloc(sizeof(*r));
    if (!r) return NULL;
    r->fp = fopen(path, "r");
    if (!r->fp) {
        free(r);
        return NULL;
    }
    r->line = NULL;
    r->linecap = 0;
    return r;
}

static char *strndup_local(const char *s, size_t n) {
    char *p = malloc(n + 1);
    if (!p) return NULL;
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

char **csv_read_row(CSVReader *r, size_t *out_count) {
    if (!r || !r->fp) return NULL;
    ssize_t linelen = 0;
    while (1) {
        linelen = getline(&r->line, &r->linecap, r->fp);
        if (linelen == -1) return NULL;
        while (linelen > 0 && (r->line[linelen-1] == '\n' || r->line[linelen-1] == '\r')) {
            r->line[--linelen] = '\0';
        }
        if (linelen == 0) continue;
        break;
    }

    char **fields = NULL;
    size_t fields_cap = 0;
    size_t fields_count = 0;

    char *p = r->line;
    char *end = r->line + linelen;

    while (p <= end) {
        char *field_start = NULL;
        char *field = NULL;
        size_t field_len = 0;

        if (*p == '"') {
            p++;
            field_start = p;
            char *dst = malloc(linelen + 1);
            if (!dst) goto fail;
            size_t dsti = 0;
            while (p < end) {
                if (*p == '"') {
                    if (p + 1 <= end && *(p+1) == '"') {
                        dst[dsti++] = '"';
                        p += 2;
                        continue;
                    } else {
                        p++;
                        break;
                    }
                }
                dst[dsti++] = *p++;
            }
            dst[dsti] = '\0';
            field = dst;

            while (p < end && *p != ';') p++;
            if (p < end && *p == ';') p++;
        } else {
            field_start = p;
            char *sep = memchr(p, ';', end - p);
            if (sep) {
                field_len = (size_t)(sep - p);
                field = strndup_local(p, field_len);
                p = sep + 1;
            } else {
                field_len = (size_t)(end - p);
                field = strndup_local(p, field_len);
                p = end + 1;
            }
        }

        if (fields_count + 1 > fields_cap) {
            size_t newcap = fields_cap == 0 ? 8 : fields_cap * 2;
            char **tmp = realloc(fields, newcap * sizeof(char*));
            if (!tmp) {
                free(field);
                goto fail;
            }
            fields = tmp;
            fields_cap = newcap;
        }
        fields[fields_count++] = field ? field : strdup("");

        if (p > end) break;
    }

    if (fields_count == 0) {
        *out_count = 0;
        return NULL;
    }
    char **res = realloc(fields, fields_count * sizeof(char*));
    if (res) fields = res;
    *out_count = fields_count;
    return fields;

fail:
    if (fields) {
        for (size_t i = 0; i < fields_count; ++i) free(fields[i]);
        free(fields);
    }
    return NULL;
}

void csv_free_fields(char **fields, size_t count) {
    if (!fields) return;
    for (size_t i = 0; i < count; ++i) {
        free(fields[i]);
    }
    free(fields);
}

void csv_close(CSVReader *r) {
    if (!r) return;
    if (r->fp) fclose(r->fp);
    free(r->line);
    free(r);
}

char **csv_load_first_column(const char *path, size_t *out_count) {
    if (out_count) *out_count = 0;
    CSVReader *r = csv_open(path);
    if (!r) return NULL;

    char **arr = NULL;
    size_t arr_cap = 0;
    size_t arr_count = 0;

    while (1) {
        size_t cols = 0;
        char **row = csv_read_row(r, &cols);
    if (!row) break;
        if (cols > 0) {
            if (arr_count + 1 > arr_cap) {
                size_t newcap = arr_cap == 0 ? 8 : arr_cap * 2;
                char **tmp = realloc(arr, newcap * sizeof(char*));
                    if (!tmp) {
                        csv_free_fields(row, cols);
                        goto cleanup;
                    }
                arr = tmp;
                arr_cap = newcap;
            }
            arr[arr_count++] = strdup(row[0]);
        }
        csv_free_fields(row, cols);
    }

    if (out_count) *out_count = arr_count;
    if (arr_count == 0) {
        free(arr);
        csv_close(r);
        return NULL;
    }
    char **res = realloc(arr, arr_count * sizeof(char*));
    if (res) arr = res;
    csv_close(r);
    return arr;

cleanup:
    if (arr) {
        for (size_t i = 0; i < arr_count; ++i) free(arr[i]);
        free(arr);
    }
    csv_close(r);
    return NULL;
}