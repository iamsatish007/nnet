#ifndef NN_LOSSES_H
#define NN_LOSSES_H

typedef enum {
    NN_LOSS_MSE,
    NN_LOSS_BINARY_CROSS_ENTROPY,
    NN_LOSS_CROSS_ENTROPY
} nn_loss_t;

// Compute scalar loss over the output vector
float nn_loss(float *predicted, float *expected, int size, nn_loss_t type);

// Compute gradient dL/d_predicted, written into out_grad
void nn_loss_grad(float *predicted, float *expected, float *out_grad,
                  int size, nn_loss_t type);

#endif // NN_LOSSES_H
