#include "nnet/core/activations.h"
#include <math.h>

float nn_activate(float x, nn_activation_t type) {
    switch (type) {
        case NN_SIGMOID:  return 1.0f / (1.0f + expf(-x));
        case NN_RELU:     return x > 0.0f ? x : 0.0f;
        case NN_TANH:     return tanhf(x);
        case NN_LINEAR:   return x;
        case NN_SOFTMAX:  return x; // softmax is handled as a vector op
        default:          return x;
    }
}

float nn_activate_deriv(float out, nn_activation_t type) {
    switch (type) {
        case NN_SIGMOID:  return out * (1.0f - out);
        case NN_RELU:     return out > 0.0f ? 1.0f : 0.0f;
        case NN_TANH:     return 1.0f - out * out;
        case NN_LINEAR:   return 1.0f;
        case NN_SOFTMAX:  return 1.0f; // handled via loss pairing
        default:          return 1.0f;
    }
}

void nn_softmax(float *input, float *output, int size) {
    // Numerically stable: subtract max first
    float max_val = input[0];
    for (int i = 1; i < size; i++) {
        if (input[i] > max_val) max_val = input[i];
    }

    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        output[i] = expf(input[i] - max_val);
        sum += output[i];
    }
    for (int i = 0; i < size; i++) {
        output[i] /= sum;
    }
}
