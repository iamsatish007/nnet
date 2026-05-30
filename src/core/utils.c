#include "nnet/core/utils.h"
#include <stdlib.h>

/* ── Helper: column-wise min/max normalisation ────────────────────────── */

void nn_normalise(float **X, int n_samples, int n_features,
                      float *mins, float *maxs) {
    /* Compute mins and maxs */
    for (int f = 0; f < n_features; f++) {
        mins[f] = X[0][f];
        maxs[f] = X[0][f];
    }
    for (int i = 1; i < n_samples; i++) {
        for (int f = 0; f < n_features; f++) {
            if (X[i][f] < mins[f]) mins[f] = X[i][f];
            if (X[i][f] > maxs[f]) maxs[f] = X[i][f];
        }
    }
    /* Scale each sample */
    for (int i = 0; i < n_samples; i++) {
        for (int f = 0; f < n_features; f++) {
            float range = maxs[f] - mins[f];
            X[i][f] = (range > 1e-8f) ? (X[i][f] - mins[f]) / range : 0.0f;
        }
    }
}

/* ── Helper: argmax over a float array ────────────────────────────────── */

int nn_argmax(const float *v, int n) {
    int best = 0;
    for (int i = 1; i < n; i++)
        if (v[i] > v[best]) best = i;
    return best;
}

/* ── Helper: Fisher-Yates shuffle on a pointer pair ──────────────────── */

void shuffle_pair(float **X, float **Y, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        float *tx = X[i]; X[i] = X[j]; X[j] = tx;
        float *ty = Y[i]; Y[i] = Y[j]; Y[j] = ty;
    }
}

/* ── Pretty-print a separator line ───────────────────────────────────── */

void print_sep(char c, int width) {
    for (int i = 0; i < width; i++) putchar(c);
    putchar('\n');
}
