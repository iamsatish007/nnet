#ifndef NN_DATA_H
#define NN_DATA_H

typedef struct {
    float **rows;     // 2D array: rows[i][j]
    int    num_rows;
    int    num_cols;
} nn_dataset_t;

// Load a CSV file into a dataset.
//   has_header: if non-zero, the first line is skipped
nn_dataset_t nn_load_csv(const char *filepath, int has_header);

// Free all memory owned by a dataset
void nn_free_dataset(nn_dataset_t *ds);

// Print a short summary of the dataset and preview up to `max_rows` rows.
void nn_print_dataset_info(const nn_dataset_t *ds, int max_rows);

// Split a dataset into input (X) and target (Y) arrays.
//   The last `num_outputs` columns become Y; the rest become X.
//   *X and *Y are allocated arrays of float pointers (one per row).
//   *input_size and *output_size are set to the column counts.
void nn_split_xy(nn_dataset_t *ds, int num_outputs,
                 float ***X, float ***Y, int *input_size, int *output_size);

#endif // NN_DATA_H
