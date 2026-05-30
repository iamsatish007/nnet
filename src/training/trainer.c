#include "nnet/training/trainer.h"
#include "nnet/core/losses.h"
#include <stdio.h>
#include <stdlib.h>

// Fisher-Yates shuffle for an index array
static void shuffle_indices(int *indices, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }
}

void nn_train(nn_network_t *nn, float **X, float **Y,
              int num_samples, nn_train_config_t config) {

    int output_size = nn->layers[nn->num_layers - 1].output_size;

    // Index array for optional shuffling
    int *indices = (int *)malloc(sizeof(int) * num_samples);
    for (int i = 0; i < num_samples; i++) indices[i] = i;

    for (int epoch = 1; epoch <= config.epochs; epoch++) {
        if (config.shuffle) {
            shuffle_indices(indices, num_samples);
        }

        float total_loss = 0.0f;

        for (int s = 0; s < num_samples; s++) {
            int idx = indices[s];

            // Forward
            nn_forward(nn, X[idx]);

            // Accumulate loss
            float *prediction = nn->layers[nn->num_layers - 1].outputs;
            total_loss += nn_loss(prediction, Y[idx], output_size, config.loss);

            // Backward
            nn_backward(nn, X[idx], Y[idx], config.learning_rate, config.loss);
        }

        // Print progress
        if (config.print_every > 0 && epoch % config.print_every == 0) {
            printf("Epoch %5d / %d  |  Loss: %.6f\n",
                   epoch, config.epochs, total_loss / num_samples);
        }
    }

    free(indices);
}
