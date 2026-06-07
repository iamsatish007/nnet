# Changelog

All notable changes to this project will be documented in this file.

This project adheres to [Semantic Versioning](https://semver.org/) and the format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

---

## [Unreleased]

### Planned
- Mini-batch training
- Adam optimizer
- Model save / load
- CNN (convolutional layers)
- RNN / LSTM / GRU
- Transformer (attention, embeddings)

---

## [0.2.0] — 2026-06-07

### Removed — ⚠️ Breaking
- **Data module removed** — `nn_load_csv`, `nn_split_xy`, `nn_free_xy`, `nn_free_dataset`, `nn_print_dataset_info`, and the `nn_dataset_t` type have been removed. The library now focuses exclusively on neural network computation. Users should handle data loading and preprocessing in their own application code.
- Removed `src/data/data.c`, `include/nnet/data/data.h`, and the `#include "data/data.h"` line from the umbrella header `nnet.h`.

### Migration guide
If you were using the data module, replace calls like:
```c
nn_dataset_t ds = nn_load_csv("data.csv", 1);
nn_split_xy(&ds, 1, &X, &Y, &in, &out);
```
with your own CSV parsing or data preparation code. The training API (`nn_train`) is unchanged — it still accepts `float **X` and `float **Y`.

---

## [0.1.0] — 2026-05-31

### Added
- **MLP (fully-connected) network** — `nn_create`, `nn_forward`, `nn_backward`, `nn_predict`, `nn_free`
- **Dense layer** with Xavier/Glorot weight initialisation — `nn_create_layer`, `nn_free_layer`
- **Activations** — Sigmoid, ReLU, Tanh, Linear, Softmax (numerically stable)
- **Loss functions** — MSE, Binary Cross-Entropy, Categorical Cross-Entropy (with ε-clipping)
- **Softmax + Cross-Entropy fused gradient** to avoid numerical instability
- **High-level training loop** — `nn_train` with epoch shuffling (Fisher-Yates), configurable print interval, and per-epoch loss reporting
- **CSV data loader** — `nn_load_csv`, `nn_split_xy`, `nn_free_xy`, `nn_free_dataset`, `nn_print_dataset_info`
- **Utility helpers** — `nn_normalise` (min-max, column-wise), `nn_argmax`, `print_sep`
- **Compile-time logging** — `NN_LOG_LEVEL` macro (0 = silent, 1 = info, 2 = debug)
- **CMake build system** — static library target `nnet::nnet`, FetchContent / add_subdirectory / system-install support, optional `NNET_BUILD_EXAMPLES` flag
- **CMake package config** — `find_package(nnet)` and `nnetConfigVersion.cmake` with `SameMajorVersion` compatibility
- **Example** — `examples/mlp_xor`: XOR solved with a 2→4→1 MLP
- NULL-guards on every `malloc`/`realloc` call; fails fast with a clear stderr message on OOM
- **CONTRIBUTING.md** Added

### Removed
- `shuffle_pair` — dead code; superseded by the private `shuffle_indices` inside `trainer.c` which uses a non-destructive index-based approach

### Notes
- Requires a C99 compiler (gcc or clang) and CMake 3.14+
- No dependencies beyond the standard math library (`libm`)

[Unreleased]: https://github.com/iamsatish007/nnet/compare/v0.2.0...HEAD
[0.2.0]: https://github.com/iamsatish007/nnet/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/iamsatish007/nnet/releases/tag/v0.1.0
