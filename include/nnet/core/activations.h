#ifndef NN_ACTIVATIONS_H
#define NN_ACTIVATIONS_H

typedef enum {
    NN_SIGMOID,
    NN_RELU,
    NN_TANH,
    NN_SOFTMAX,
    NN_LINEAR
} nn_activation_t;

// Apply activation to a single value (not for softmax)
float nn_activate(float x, nn_activation_t type);

// Derivative of activation given the *already-activated* output
float nn_activate_deriv(float activated_output, nn_activation_t type);

// Softmax is a vector operation — applied to an entire layer
void nn_softmax(float *input, float *output, int size);

#endif // NN_ACTIVATIONS_H
