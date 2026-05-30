#ifndef NN_SEQUENTIAL_H
#define NN_SEQUENTIAL_H

#include "../layers/dense.h"
#include "../core/losses.h"

typedef struct {
    int num_layers;       // number of weight layers (= num_sizes - 1)
    nn_layer_t *layers;
} nn_network_t;

// Create a network.
//   num_sizes:   length of the sizes array (e.g. 3 for {2,4,1})
//   sizes:       neuron count for each logical layer including input
//   activations: activation per weight-layer (length = num_sizes - 1)
nn_network_t nn_create(int num_sizes, int *sizes, nn_activation_t *activations);

// Run a forward pass; results stored in each layer's outputs
void nn_forward(nn_network_t *nn, float *inputs);

// Run backpropagation and update weights
//   inputs:   the original input vector for this sample
//   expected: target output vector (length = output layer size)
void nn_backward(nn_network_t *nn, float *inputs, float *expected,
                 float learning_rate, nn_loss_t loss);

// Convenience: forward pass + return pointer to the output layer's outputs
// (pointer is valid until the next forward call)
float *nn_predict(nn_network_t *nn, float *inputs);

// Free all network memory
void nn_free(nn_network_t *nn);

#endif // NN_SEQUENTIAL_H
