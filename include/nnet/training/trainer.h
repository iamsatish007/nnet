#ifndef NN_TRAINER_H
#define NN_TRAINER_H

#include "../network/sequential.h"

typedef struct {
    int       epochs;
    float     learning_rate;
    nn_loss_t loss;
    int       print_every;   // print loss info every N epochs, 0 = silent
    int       shuffle;       // 1 = shuffle samples each epoch
} nn_train_config_t;

// High-level training loop.
//   X: array of input vectors   (num_samples pointers)
//   Y: array of target vectors  (num_samples pointers)
void nn_train(nn_network_t *nn, float **X, float **Y,
              int num_samples, nn_train_config_t config);

#endif // NN_TRAINER_H
