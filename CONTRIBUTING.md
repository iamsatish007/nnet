# Contributing to nnet

Thank you for taking the time to contribute! 🎉  
All contributions — bug reports, documentation improvements, new features, and code reviews — are valued and appreciated.

> **By contributing you agree that your work will be released under the project's [MIT License](LICENSE).**

---

## Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [Ways to Contribute](#ways-to-contribute)
3. [Getting Started](#getting-started)
4. [Development Workflow](#development-workflow)
5. [Coding Standards](#coding-standards)
6. [Commit Message Convention](#commit-message-convention)
7. [Pull Request Guidelines](#pull-request-guidelines)
8. [Reporting Bugs](#reporting-bugs)
9. [Requesting Features](#requesting-features)
10. [Documentation](#documentation)
11. [Versioning](#versioning)
12. [Communication](#communication)

---

## Code of Conduct

This project follows the [Contributor Covenant Code of Conduct v2.1](https://www.contributor-covenant.org/version/2/1/code_of_conduct/).  
By participating you are expected to uphold this code. Please report unacceptable behaviour to the maintainer via a GitHub private message or by opening a [GitHub Security Advisory](https://docs.github.com/en/code-security/security-advisories/working-with-repository-security-advisories/creating-a-repository-security-advisory).

---

## Ways to Contribute

| Type | How |
|---|---|
| 🐛 Bug report | Open a [GitHub Issue](https://github.com/iamsatish007/nnet/issues/new?template=bug_report.md) |
| 💡 Feature request | Open a [GitHub Issue](https://github.com/iamsatish007/nnet/issues/new?template=feature_request.md) |
| 📖 Documentation | Submit a pull request editing Markdown files or header-level doc comments |
| 🔧 Code fix / feature | Fork → branch → PR (see [Development Workflow](#development-workflow)) |
| ✅ Code review | Leave comments on open pull requests |
| 💬 Discussion | Use [GitHub Discussions](https://github.com/iamsatish007/nnet/discussions) for questions and ideas |

---

## Getting Started

### Prerequisites

| Tool | Minimum version |
|---|---|
| C compiler (gcc **or** clang) | gcc 9 / clang 11 |
| CMake | 3.14 |
| Standard math library (`libm`) | — (ships with the compiler) |
| Git | 2.x |

> **Note:** nnet has **no third-party runtime dependencies**. You do not need to install anything beyond the standard toolchain.

### Fork and clone

```bash
# 1. Fork the repository via the GitHub UI, then:
git clone https://github.com/<your-username>/nnet.git
cd nnet

# 2. Add the upstream remote so you can keep your fork in sync
git remote add upstream https://github.com/iamsatish007/nnet.git
```

### Build

```bash
cmake -B build -S . -DNNET_BUILD_EXAMPLES=ON
cmake --build build
```

The static library is produced at `build/libnnet.a`.  
Example binaries are placed under `build/examples/`.

### Verify the build

```bash
./build/examples/mlp_xor/mlp_xor
```

Expected output (values will vary slightly due to random initialisation):

```
[0, 0] -> 0.0312
[0, 1] -> 0.9701
[1, 0] -> 0.9698
[1, 1] -> 0.0289
```

---

## Development Workflow

We follow a **fork-and-branch** model.

```
main  ←── your PR
 │
 └── you fork → feature/my-improvement
```

### Step-by-step

```bash
# 1. Sync your fork with upstream
git fetch upstream
git checkout main
git merge --ff-only upstream/main

# 2. Create a focused branch
#    Use one of the prefixes below (see Commit Convention)
git checkout -b feat/adam-optimizer
# or: fix/, docs/, refactor/, test/, chore/

# 3. Make your changes, commit often (small logical commits)
git add -p          # stage only what belongs to this change
git commit

# 4. Keep your branch current
git fetch upstream
git rebase upstream/main

# 5. Push and open a PR
git push origin feat/adam-optimizer
```

> **One concern per branch.** Do not mix unrelated fixes in a single PR. This keeps history readable and review fast.

---

## Coding Standards

nnet is written in **C99** with no extensions (`-std=c99`). Please respect the following rules so the project stays consistent and portable.

### General

- Target **C99** strictly. Do not use compiler-specific extensions or GNU-isms unless gated behind a feature macro.
- All public API symbols must be prefixed with `nn_` (e.g., `nn_create`, `nn_train_config_t`).
- Every `malloc` / `realloc` call **must** be NULL-checked. On failure, print a descriptive message to `stderr` and `exit(EXIT_FAILURE)`.
- Avoid global mutable state. Prefer passing state through function parameters.

### Style

- **Indentation:** 4 spaces — no tabs.
- **Braces:** K&R style — opening brace on the same line as the control statement.
- **Line length:** Soft limit of 100 characters.
- **Naming:**
  - `snake_case` for variables, functions, and struct fields.
  - `UPPER_SNAKE_CASE` for macros and enum constants.
  - Struct and typedef names end in `_t` (e.g., `nn_network_t`).
- **Header guards:** Use `#ifndef NN_<FILENAME>_H` / `#define` / `#endif` — no `#pragma once`.
- **Comments:** Use `/* … */` for block comments and `//` for single-line comments. All public functions in headers must have a brief doc comment.

### Example (correct style)

```c
/*
 * nn_argmax — return the index of the largest element in v[0..n-1].
 */
int nn_argmax(const float *v, int n) {
    int best = 0;
    for (int i = 1; i < n; i++) {
        if (v[i] > v[best]) {
            best = i;
        }
    }
    return best;
}
```

### Compiler warnings

Your code must compile cleanly under:

```bash
gcc -std=c99 -Wall -Wextra -Wpedantic -O2
```

Zero warnings are required; do not suppress warnings with pragmas or casts unless you add an explanation comment.

---

## Commit Message Convention

We follow the **[Conventional Commits 1.0.0](https://www.conventionalcommits.org/en/v1.0.0/)** specification.

```
<type>(<scope>): <short summary>

[optional body]

[optional footer(s)]
```

### Types

| Type | When to use |
|---|---|
| `feat` | A new feature visible to users |
| `fix` | A bug fix |
| `docs` | Documentation only |
| `refactor` | Code restructuring without behaviour change |
| `test` | Adding or updating tests |
| `perf` | Performance improvement |
| `chore` | Build system, CI, tooling, dependencies |
| `revert` | Revert a previous commit |

### Scopes (optional but encouraged)

`core`, `layers`, `network`, `training`, `data`, `cmake`, `examples`, `docs`

### Rules

- **Summary:** imperative mood, lowercase, no period, ≤ 72 characters.  
  ✅ `feat(training): add adam optimizer`  
  ❌ `Added Adam Optimizer.`
- **Breaking change:** append `!` after the type/scope **and** add a `BREAKING CHANGE:` footer.  
  `feat(network)!: rename nn_create to nn_network_create`
- **Reference issues:** close with `Closes #<number>` or `Fixes #<number>` in the footer.

### Example

```
fix(training): prevent division by zero in cross-entropy loss

Add epsilon clipping (1e-7) to the log() call in nn_loss_cross_entropy
to avoid NaN gradients when a softmax output is exactly 0.

Fixes #42
```

---

## Pull Request Guidelines

### Before you open a PR

- [ ] Your branch is rebased on the latest `upstream/main`.
- [ ] The project builds cleanly with `-Wall -Wextra -Wpedantic`.
- [ ] You have manually tested your change (run the XOR example at minimum).
- [ ] New public functions have a doc comment in the header.
- [ ] `CHANGELOG.md` has an entry under `[Unreleased]` describing your change.
- [ ] Commit messages follow the [Conventional Commits](#commit-message-convention) spec.

### PR description

Use the following template when opening a pull request:

```markdown
## What does this PR do?
<!-- A concise description of the change and its motivation. -->

## How was it tested?
<!-- Describe the steps you took to verify correctness. -->

## Checklist
- [ ] Builds without warnings (`-Wall -Wextra -Wpedantic`)
- [ ] Manually tested
- [ ] Header doc comments added/updated
- [ ] CHANGELOG.md updated
- [ ] Commits follow Conventional Commits

## Related issues
Closes #
```

### Review process

1. A maintainer will review your PR within **30 days** of submission.
2. Address all requested changes; push new commits (do not force-push during an active review unless asked).
3. Once approved, the maintainer will **squash-merge** the PR into `main`.

> Small, focused PRs are reviewed and merged much faster than large, sweeping ones.

---

## Reporting Bugs

Before filing a bug report:

1. Search [existing issues](https://github.com/iamsatish007/nnet/issues) to avoid duplicates.
2. Try to reproduce the issue on the latest `main` branch.

A good bug report includes:

- **nnet version** (output of `git describe --tags`) and compiler + OS details.
- **Minimal reproducible example** — the shortest C snippet or CMake config that triggers the bug.
- **Expected behaviour** vs **actual behaviour**.
- **Full compiler output or error log.**

**Security vulnerabilities** must **not** be filed as public issues. Please report them privately via a [GitHub Security Advisory](https://docs.github.com/en/code-security/security-advisories).

---

## Requesting Features

Feature requests are welcomed. Please:

1. Search [existing issues](https://github.com/iamsatish007/nnet/issues) and [Discussions](https://github.com/iamsatish007/nnet/discussions) first.
2. Open an issue with the **`enhancement`** label and describe:
   - The problem you are solving.
   - Your proposed API or behaviour (pseudocode is fine).
   - Any trade-offs you are aware of.

Items on the [roadmap](README.md#roadmap) (mini-batch training, Adam, CNN, RNN, Transformer) are high priority; contributions toward these are especially welcome.

---

## Documentation

Documentation lives alongside the code:

- **`README.md`** — integration guide, quick start, API reference.
- **`CHANGELOG.md`** — updated with every change (Keep a Changelog format).
- **Header files** (`include/nnet/`) — source-of-truth doc comments for every public symbol.

When adding or changing a public function:

1. Update or add the doc comment in the relevant header file.
2. Update `README.md` if the public API reference section is affected.
3. Add a `CHANGELOG.md` entry under `[Unreleased]`.

---

## Versioning

nnet follows **[Semantic Versioning 2.0.0](https://semver.org/)**:

| Bump | When |
|---|---|
| **PATCH** `0.1.x` | Backwards-compatible bug fixes |
| **MINOR** `0.x.0` | Backwards-compatible new features |
| **MAJOR** `x.0.0` | Breaking API changes |

While the project is in the `0.x` series, minor version bumps may include breaking changes with a notice in `CHANGELOG.md`.

---

## Communication

| Channel | Purpose |
|---|---|
| [GitHub Issues](https://github.com/iamsatish007/nnet/issues) | Bug reports, concrete feature requests |
| [GitHub Discussions](https://github.com/iamsatish007/nnet/discussions) | Questions, design ideas, general chat |
| Pull Request comments | Code-specific review discussion |

Please be patient — this is a solo-maintained project. Response times may vary.

---

*Thank you again for contributing to nnet. Every improvement, no matter how small, makes the project better for everyone.* 🙏
