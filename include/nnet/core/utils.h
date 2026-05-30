#ifndef NN_UTILS_H
#define NN_UTILS_H

#include <stdio.h>

/* ── Logging ──────────────────────────────────────────────────────────── */

#ifndef NN_LOG_LEVEL
#define NN_LOG_LEVEL 1
#endif

#define LOG_INFO(fmt, ...)  do { if (NN_LOG_LEVEL >= 1) \
    fprintf(stdout, "[INFO]  " fmt "\n", ##__VA_ARGS__); } while(0)
#define LOG_DEBUG(fmt, ...) do { if (NN_LOG_LEVEL >= 2) \
    fprintf(stdout, "[DEBUG] " fmt "\n", ##__VA_ARGS__); } while(0)
#define LOG_WARN(fmt, ...)  fprintf(stderr, "[WARN]  " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

/* ── Helper Functions ─────────────────────────────────────────────────── */

void normalise(float **X, int n_samples, int n_features, float *mins, float *maxs);
int argmax(const float *v, int n);
void shuffle_pair(float **X, float **Y, int n);
void print_sep(char c, int width);

#endif // NN_UTILS_H
