# nnet

A neural network library written in C99. No dependencies beyond the standard math library (`libm`).

Currently supports fully-connected (MLP) networks. CNN, RNN, and Transformer support is planned.

---

## Requirements

- C99 compiler (gcc or clang)
- CMake 3.14+
- Standard math library (`libm`)

---

## Integration

### Option 1 — CMake FetchContent (recommended)

The simplest way. CMake downloads, configures, and builds nnet automatically.

```cmake
cmake_minimum_required(VERSION 3.14)
project(myapp C)

include(FetchContent)

FetchContent_Declare(
    nnet
    GIT_REPOSITORY https://github.com/iamsatish007/nnet.git
    GIT_TAG        v0.1.0
)
FetchContent_MakeAvailable(nnet)

add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE nnet::nnet)
```

> **Tip:** Pin to a release tag (e.g. `v0.1.0`) rather than `main` so your build stays reproducible.

### Option 2 — CMake add_subdirectory

Clone the repo into your project tree and reference it locally:

```bash
git clone https://github.com/iamsatish007/nnet.git third_party/nnet
```

```cmake
add_subdirectory(third_party/nnet)

add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE nnet::nnet)
```

### Option 3 — System install

Build and install once, then use from any project:

```bash
git clone https://github.com/iamsatish007/nnet.git
cd nnet
cmake -B build -DNNET_BUILD_EXAMPLES=OFF
cmake --build build
sudo cmake --install build
```

Use with `find_package`:

```cmake
find_package(nnet REQUIRED)
target_link_libraries(myapp PRIVATE nnet::nnet)
```

Or compile directly:

```bash
gcc main.c -lnnet -lm -o myapp
```

---

## Quick start

```c
#include <nnet/nnet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand(time(NULL));

    // 2 inputs → 4 hidden (ReLU) → 1 output (Sigmoid)
    int sizes[]            = {2, 4, 1};
    nn_activation_t acts[] = {NN_RELU, NN_SIGMOID};
    nn_network_t net       = nn_create(3, sizes, acts);

    // XOR dataset
    float x0[] = {0,0}, x1[] = {0,1}, x2[] = {1,0}, x3[] = {1,1};
    float y0[] = {0},   y1[] = {1},   y2[] = {1},   y3[] = {0};
    float *X[] = {x0, x1, x2, x3};
    float *Y[] = {y0, y1, y2, y3};

    // Train
    nn_train_config_t cfg = {
        .epochs        = 10000,
        .learning_rate = 0.5f,
        .loss          = NN_LOSS_MSE,
        .print_every   = 2000,
        .shuffle       = 1,
    };
    nn_train(&net, X, Y, 4, cfg);

    // Predict
    for (int i = 0; i < 4; i++) {
        float *out = nn_predict(&net, X[i]);
        printf("[%.0f, %.0f] -> %.4f\n", X[i][0], X[i][1], out[0]);
    }

    nn_free(&net);
    return 0;
}
```

---

## Loading data from CSV

```c
// Load CSV (1 = skip header row)
nn_dataset_t ds = nn_load_csv("data/iris.csv", 1);
nn_print_dataset_info(&ds, 5);

// Split into inputs X and targets Y
// Last `num_outputs` columns become Y; the rest become X
float **X, **Y;
int input_size, output_size;
nn_split_xy(&ds, 1, &X, &Y, &input_size, &output_size);

// ... train ...

// Free X and Y first (before nn_free_dataset clears num_rows)
nn_free_xy(X, Y, ds.num_rows);
nn_free_dataset(&ds);
```

---

## API reference

### Network

```c
// Create a sequential network.
//   num_sizes:   length of sizes[] — e.g. 3 for a {2, 4, 1} network
//   sizes:       neuron count per layer, including the input layer
//   activations: one activation per weight layer (length = num_sizes - 1)
nn_network_t nn_create(int num_sizes, int *sizes, nn_activation_t *activations);

float  *nn_predict(nn_network_t *nn, float *input);  // forward pass; returns output pointer
void    nn_forward(nn_network_t *nn, float *input);  // forward pass (in-place)
void    nn_backward(nn_network_t *nn, float *input, float *expected,
                    float learning_rate, nn_loss_t loss);
void    nn_free(nn_network_t *nn);
```

### Training

```c
typedef struct {
    int       epochs;
    float     learning_rate;
    nn_loss_t loss;
    int       print_every;   // 0 = silent
    int       shuffle;       // 1 = shuffle each epoch
} nn_train_config_t;

void nn_train(nn_network_t *nn, float **X, float **Y,
              int num_samples, nn_train_config_t config);
```

### Activations

| Constant | Function |
|---|---|
| `NN_SIGMOID` | 1 / (1 + e^-x) |
| `NN_RELU` | max(0, x) |
| `NN_TANH` | tanh(x) |
| `NN_LINEAR` | x (identity) |
| `NN_SOFTMAX` | softmax (vector operation; use on the output layer) |

### Losses

| Constant | Use case |
|---|---|
| `NN_LOSS_MSE` | Regression |
| `NN_LOSS_BINARY_CROSS_ENTROPY` | Binary classification (pair with `NN_SIGMOID`) |
| `NN_LOSS_CROSS_ENTROPY` | Multi-class classification (pair with `NN_SOFTMAX`) |

### Data

```c
nn_dataset_t nn_load_csv(const char *filepath, int has_header);
void         nn_free_dataset(nn_dataset_t *ds);
void         nn_print_dataset_info(const nn_dataset_t *ds, int max_rows);
void         nn_split_xy(nn_dataset_t *ds, int num_outputs,
                         float ***X, float ***Y,
                         int *input_size, int *output_size);

// Free the X and Y arrays allocated by nn_split_xy.
// Call this before nn_free_dataset (pass ds.num_rows before it is cleared).
void         nn_free_xy(float **X, float **Y, int num_rows);
```

### Utilities

```c
// Min-max normalise each feature column in-place.
// mins and maxs must be pre-allocated arrays of length n_features.
void nn_normalise(float **X, int n_samples, int n_features, float *mins, float *maxs);

// Return the index of the largest element (for classification output).
int nn_argmax(const float *v, int n);
```

### Logging

nnet prints training progress to `stdout`. Verbosity is controlled at compile time:

```c
#define NN_LOG_LEVEL 0   // 0 = silent, 1 = info (default), 2 = debug
#include <nnet/nnet.h>
```

Or pass it via the compiler:

```bash
gcc main.c -DNN_LOG_LEVEL=0 -lnnet -lm -o myapp
```

```cmake
target_compile_definitions(myapp PRIVATE NN_LOG_LEVEL=0)
```

---

## Building from source

```bash
git clone https://github.com/iamsatish007/nnet.git
cd nnet
cmake -B build -S .
cmake --build build
# Static library: build/libnnet.a
```

### Building examples

```bash
cmake -B build -DNNET_BUILD_EXAMPLES=ON
cmake --build build
./build/examples/mlp_xor/mlp_xor
```

### Uninstalling after a system install

```bash
sudo cmake --build build --target uninstall
```

---

## Examples

| Example | Description |
|---|---|
| [`examples/mlp_xor`](examples/mlp_xor) | XOR with a 2→4→1 MLP (sigmoid activations, MSE loss) |

---

## Roadmap

- [x] MLP (fully-connected layers)
- [ ] Mini-batch training
- [ ] Adam optimizer
- [ ] Model save / load
- [ ] CNN (convolutional layers)
- [ ] RNN / LSTM / GRU
- [ ] Transformer (attention, embeddings)

---

## License

MIT
