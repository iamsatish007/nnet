#ifndef NN_DENSE_H
#define NN_DENSE_H

#include "../core/activations.h"

typedef struct {
    int input_size;
    int output_size;
    nn_activation_t activation;
    float *weights;    // [input_size * output_size], row-major
    float *biases;     // [output_size]
    float *z_values;   // pre-activation sums  [output_size]
    float *outputs;    // post-activation values [output_size]
    float *deltas;     // error gradients [output_size]
} nn_layer_t;

// Create a layer connecting input_size neurons to output_size neurons
nn_layer_t nn_create_layer(int input_size, int output_size, nn_activation_t act);

// Free all memory owned by a layer
void nn_free_layer(nn_layer_t *layer);

#endif // NN_DENSE_H
