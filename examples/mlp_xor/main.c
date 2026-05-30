#include <nnet/nnet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  srand(time(NULL));

  // ── 1. Define the network architecture ──────────────────────
  //   2 inputs → 4 hidden (sigmoid) → 1 output (sigmoid)
  int sizes[] = {2, 4, 1};
  nn_activation_t activations[] = {NN_SIGMOID, NN_SIGMOID};

  nn_network_t nn = nn_create(3, sizes, activations);

  // ── 2. XOR dataset ──────────────────────────────────────────
  float x0[] = {0, 0};
  float x1[] = {0, 1};
  float x2[] = {1, 0};
  float x3[] = {1, 1};

  float y0[] = {0};
  float y1[] = {1};
  float y2[] = {1};
  float y3[] = {0};

  float *X[] = {x0, x1, x2, x3};
  float *Y[] = {y0, y1, y2, y3};

  // ── 3. Train ────────────────────────────────────────────────
  nn_train_config_t config = {.epochs = 10000,
                              .learning_rate = 0.5f,
                              .loss = NN_LOSS_MSE,
                              .print_every = 2000,
                              .shuffle = 1};

  printf("Training XOR network...\n");
  printf("─────────────────────────────────────────\n");
  nn_train(&nn, X, Y, 4, config);

  // ── 4. Test ─────────────────────────────────────────────────
  printf("\nResults:\n");
  printf("─────────────────────────────────────────\n");
  for (int i = 0; i < 4; i++) {
    float *out = nn_predict(&nn, X[i]);
    printf("  [%.0f, %.0f]  →  expected %.0f  |  got %.4f\n", X[i][0], X[i][1],
           Y[i][0], out[0]);
  }

  // ── 5. Cleanup ──────────────────────────────────────────────
  nn_free(&nn);
  return 0;
}
