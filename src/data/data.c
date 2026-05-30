#include "nnet/data/data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NN_CSV_MAX_LINE 4096

// Count comma-separated columns in a line
static int count_cols(const char *line) {
    int cols = 1;
    for (const char *p = line; *p; p++) {
        if (*p == ',') cols++;
    }
    return cols;
}

nn_dataset_t nn_load_csv(const char *filepath, int has_header) {
    nn_dataset_t ds = {NULL, 0, 0};

    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        fprintf(stderr, "[nn] Error: could not open file '%s'\n", filepath);
        return ds;
    }

    char line[NN_CSV_MAX_LINE];

    // Skip header if present
    if (has_header) {
        if (!fgets(line, sizeof(line), fp)) {
            fclose(fp);
            return ds;
        }
    }

    // First pass: count rows and determine column count
    int capacity = 64;
    int num_rows = 0;
    int num_cols = 0;
    float **rows = (float **)malloc(sizeof(float *) * capacity);

    while (fgets(line, sizeof(line), fp)) {
        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;

        if (num_cols == 0) {
            num_cols = count_cols(line);
        }

        // Parse columns
        float *row = (float *)malloc(sizeof(float) * num_cols);
        char *token = strtok(line, ",\n\r");
        int col = 0;
        while (token && col < num_cols) {
            row[col++] = (float)atof(token);
            token = strtok(NULL, ",\n\r");
        }

        // Grow array if needed
        if (num_rows >= capacity) {
            capacity *= 2;
            rows = (float **)realloc(rows, sizeof(float *) * capacity);
        }
        rows[num_rows++] = row;
    }

    fclose(fp);

    ds.rows     = rows;
    ds.num_rows = num_rows;
    ds.num_cols = num_cols;
    return ds;
}

void nn_free_dataset(nn_dataset_t *ds) {
    for (int i = 0; i < ds->num_rows; i++) {
        free(ds->rows[i]);
    }
    free(ds->rows);
    ds->rows     = NULL;
    ds->num_rows = 0;
    ds->num_cols = 0;
}

void nn_print_dataset_info(const nn_dataset_t *ds, int max_rows) {
    const int max_preview_cols = 16;

    if (!ds) {
        printf("Dataset: (null)\n");
        return;
    }

    printf("Dataset info:\n");
    printf("  Rows   : %d\n", ds->num_rows);
    printf("  Columns: %d\n", ds->num_cols);

    if (!ds->rows || ds->num_rows == 0 || ds->num_cols == 0) {
        printf("  Preview: dataset is empty\n");
        return;
    }

    if (max_rows < 0) {
        max_rows = 0;
    }
    if (max_rows > ds->num_rows) {
        max_rows = ds->num_rows;
    }

    printf("  Preview (first %d row%s):\n",
           max_rows, max_rows == 1 ? "" : "s");

    for (int i = 0; i < max_rows; i++) {
        int preview_cols = ds->num_cols < max_preview_cols ? ds->num_cols : max_preview_cols;

        printf("    [%d] ", i);
        for (int j = 0; j < preview_cols; j++) {
            printf("%.6g", ds->rows[i][j]);
            if (j < preview_cols - 1) {
                printf(", ");
            }
        }
        if (preview_cols < ds->num_cols) {
            printf(", ...");
        }
        printf("\n");
    }

    if (max_rows < ds->num_rows) {
        printf("    ... (%d more row%s)\n",
               ds->num_rows - max_rows,
               (ds->num_rows - max_rows) == 1 ? "" : "s");
    }
}

void nn_split_xy(nn_dataset_t *ds, int num_outputs,
                 float ***X, float ***Y, int *input_size, int *output_size) {

    int in_cols  = ds->num_cols - num_outputs;
    int out_cols = num_outputs;

    *input_size  = in_cols;
    *output_size = out_cols;

    *X = (float **)malloc(sizeof(float *) * ds->num_rows);
    *Y = (float **)malloc(sizeof(float *) * ds->num_rows);

    for (int i = 0; i < ds->num_rows; i++) {
        (*X)[i] = (float *)malloc(sizeof(float) * in_cols);
        (*Y)[i] = (float *)malloc(sizeof(float) * out_cols);

        for (int j = 0; j < in_cols; j++) {
            (*X)[i][j] = ds->rows[i][j];
        }
        for (int j = 0; j < out_cols; j++) {
            (*Y)[i][j] = ds->rows[i][in_cols + j];
        }
    }
}
