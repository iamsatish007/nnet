#include "nnet/network/sequential.h"
#include "nnet/core/activations.h"
#include "nnet/core/losses.h"
#include <stdlib.h>
#include <stdio.h>

// ----- Creation / Destruction -----

nn_network_t nn_create(int num_sizes, int *sizes, nn_activation_t *activations) {
    nn_network_t nn;
    nn.num_layers = num_sizes - 1;
    nn.layers = (nn_layer_t *)malloc(sizeof(nn_layer_t) * nn.num_layers);

    for (int i = 0; i < nn.num_layers; i++) {
        nn.layers[i] = nn_create_layer(sizes[i], sizes[i + 1], activations[i]);
    }
    return nn;
}

void nn_free(nn_network_t *nn) {
    for (int i = 0; i < nn->num_layers; i++) {
        nn_free_layer(&nn->layers[i]);
    }
    free(nn->layers);
    nn->layers = NULL;
    nn->num_layers = 0;
}

// ----- Forward Pass -----

void nn_forward(nn_network_t *nn, float *inputs) {
    float *current_inputs = inputs;

    for (int l = 0; l < nn->num_layers; l++) {
        nn_layer_t *layer = &nn->layers[l];

        // Compute pre-activation: z = W^T * x + b
        for (int j = 0; j < layer->output_size; j++) {
            float sum = layer->biases[j];
            for (int i = 0; i < layer->input_size; i++) {
                sum += current_inputs[i] * layer->weights[i * layer->output_size + j];
            }
            layer->z_values[j] = sum;
        }

        // Apply activation
        if (layer->activation == NN_SOFTMAX) {
            nn_softmax(layer->z_values, layer->outputs, layer->output_size);
        } else {
            for (int j = 0; j < layer->output_size; j++) {
                layer->outputs[j] = nn_activate(layer->z_values[j], layer->activation);
            }
        }

        current_inputs = layer->outputs;
    }
}

// ----- Backward Pass -----

void nn_backward(nn_network_t *nn, float *inputs, float *expected,
                 float learning_rate, nn_loss_t loss) {
    int out_idx = nn->num_layers - 1;
    nn_layer_t *out_layer = &nn->layers[out_idx];

    // Temporary buffer for the loss gradient
    float *loss_grad = (float *)malloc(sizeof(float) * out_layer->output_size);
    nn_loss_grad(out_layer->outputs, expected, loss_grad,
                 out_layer->output_size, loss);

    // --- 1. Output layer deltas ---
    // Special case: softmax + cross-entropy → simplified gradient (y - t)
    if (out_layer->activation == NN_SOFTMAX && loss == NN_LOSS_CROSS_ENTROPY) {
        for (int j = 0; j < out_layer->output_size; j++) {
            out_layer->deltas[j] = out_layer->outputs[j] - expected[j];
        }
    } else {
        for (int j = 0; j < out_layer->output_size; j++) {
            out_layer->deltas[j] = loss_grad[j] *
                nn_activate_deriv(out_layer->outputs[j], out_layer->activation);
        }
    }
    free(loss_grad);

    // --- 2. Hidden layer deltas (back-propagate) ---
    for (int l = out_idx - 1; l >= 0; l--) {
        nn_layer_t *curr = &nn->layers[l];
        nn_layer_t *next = &nn->layers[l + 1];

        for (int i = 0; i < curr->output_size; i++) {
            float error_sum = 0.0f;
            for (int j = 0; j < next->output_size; j++) {
                error_sum += next->deltas[j] *
                             next->weights[i * next->output_size + j];
            }
            curr->deltas[i] = error_sum *
                nn_activate_deriv(curr->outputs[i], curr->activation);
        }
    }

    // --- 3. Update weights and biases ---
    for (int l = 0; l < nn->num_layers; l++) {
        nn_layer_t *layer = &nn->layers[l];
        float *layer_inputs = (l == 0) ? inputs : nn->layers[l - 1].outputs;

        for (int j = 0; j < layer->output_size; j++) {
            layer->biases[j] -= learning_rate * layer->deltas[j];

            for (int i = 0; i < layer->input_size; i++) {
                layer->weights[i * layer->output_size + j] -=
                    learning_rate * layer->deltas[j] * layer_inputs[i];
            }
        }
    }
}

// ----- Predict (convenience) -----

float *nn_predict(nn_network_t *nn, float *inputs) {
    nn_forward(nn, inputs);
    return nn->layers[nn->num_layers - 1].outputs;
}
