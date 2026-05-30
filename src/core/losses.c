#include "nnet/core/losses.h"
#include <math.h>

#define NN_EPSILON 1e-7f

float nn_loss(float *predicted, float *expected, int size, nn_loss_t type) {
    float loss = 0.0f;

    switch (type) {
        case NN_LOSS_MSE:
            for (int i = 0; i < size; i++) {
                float diff = predicted[i] - expected[i];
                loss += diff * diff;
            }
            return loss / size;

        case NN_LOSS_BINARY_CROSS_ENTROPY:
            for (int i = 0; i < size; i++) {
                float p = fmaxf(fminf(predicted[i], 1.0f - NN_EPSILON), NN_EPSILON);
                loss += -(expected[i] * logf(p) +
                          (1.0f - expected[i]) * logf(1.0f - p));
            }
            return loss / size;

        case NN_LOSS_CROSS_ENTROPY:
            for (int i = 0; i < size; i++) {
                float p = fmaxf(predicted[i], NN_EPSILON);
                loss += -expected[i] * logf(p);
            }
            return loss;

        default:
            return 0.0f;
    }
}

void nn_loss_grad(float *predicted, float *expected, float *out_grad,
                  int size, nn_loss_t type) {
    switch (type) {
        case NN_LOSS_MSE:
            for (int i = 0; i < size; i++) {
                out_grad[i] = 2.0f * (predicted[i] - expected[i]) / size;
            }
            break;

        case NN_LOSS_BINARY_CROSS_ENTROPY:
            for (int i = 0; i < size; i++) {
                float p = fmaxf(fminf(predicted[i], 1.0f - NN_EPSILON), NN_EPSILON);
                out_grad[i] = (p - expected[i]) / (p * (1.0f - p));
            }
            break;

        case NN_LOSS_CROSS_ENTROPY:
            for (int i = 0; i < size; i++) {
                float p = fmaxf(predicted[i], NN_EPSILON);
                out_grad[i] = -expected[i] / p;
            }
            break;
    }
}
