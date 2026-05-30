#include "nnet/layers/dense.h"
#include <stdlib.h>
#include <math.h>

nn_layer_t nn_create_layer(int input_size, int output_size, nn_activation_t act) {
    nn_layer_t layer;
    layer.input_size  = input_size;
    layer.output_size = output_size;
    layer.activation  = act;

    int num_weights = input_size * output_size;

    layer.weights  = (float *)malloc(sizeof(float) * num_weights);
    layer.biases   = (float *)malloc(sizeof(float) * output_size);
    layer.z_values = (float *)malloc(sizeof(float) * output_size);
    layer.outputs  = (float *)malloc(sizeof(float) * output_size);
    layer.deltas   = (float *)malloc(sizeof(float) * output_size);

    // Xavier / Glorot initialization: range = sqrt(6 / (in + out))
    float limit = sqrtf(6.0f / (input_size + output_size));
    for (int i = 0; i < num_weights; i++) {
        layer.weights[i] = ((float)rand() / RAND_MAX) * 2.0f * limit - limit;
    }

    for (int i = 0; i < output_size; i++) {
        layer.biases[i]   = 0.0f;
        layer.z_values[i] = 0.0f;
        layer.outputs[i]  = 0.0f;
        layer.deltas[i]   = 0.0f;
    }

    return layer;
}

void nn_free_layer(nn_layer_t *layer) {
    free(layer->weights);
    free(layer->biases);
    free(layer->z_values);
    free(layer->outputs);
    free(layer->deltas);
    layer->weights  = NULL;
    layer->biases   = NULL;
    layer->z_values = NULL;
    layer->outputs  = NULL;
    layer->deltas   = NULL;
}
