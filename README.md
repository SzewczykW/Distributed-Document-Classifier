# Distributed Document Classifier

A parallel document classification engine based on MPI, inspired by Chapter 9 of *Parallel Programming in C with MPI and OpenMP* by Michael J. Quinn.

This project implements a scalable manager-worker architecture using MPICH to classify text documents into feature vectors. It uses hashing to match words against a shared dictionary and distributes processing using MPI. It supports only `.txt` documents.

---

## Overview

- The **manager process**:
  - reads the dictionary file, which contains one keyword per line,
  - broadcasts the dictionary to all worker processes,
  - scans the input directory for `.txt` documents,
  - distributes file paths to workers,
  - receives classified feature vectors from workers and writes them to the output file.

- Each **worker process**:
  - receives the dictionary from the manager,
  - receives a file path from the manager,
  - tokenizes the document into lowercase words,
  - checks each token against the dictionary stored in a fixed-size hash table,
  - builds a feature vector representing the presence or frequency of dictionary words,
  - sends the feature vector back to the manager.
---

## Project Structure

```
.
├── include/                        # Header files
├── src/                            # Core runtime: main, manager, worker, utils
├── tests/                          # Criterion unit tests
├── docs/                           # Doxygen config + generated docs
├── make/                           # Makefile submodules (build, test, docs, run)
├── scripts/install_mpich.sh        # Script for downloading, building and installing MPICH locally
├── scripts/gen_data.py             # Script for generating random data (see ./scripts/gen_data.py --help for more)
├── Makefile                        # Entry point Makefile
├── CMakeLists.txt                  # Entry point CMake
```
---

## Requirements

- GCC 12+
- GNU Make
- MPICH 4.3.0+ (see below for installation)
- (Optional) [Doxygen](https://www.doxygen.nl) + Graphviz + LaTeX for documentation
- (Optional) [Criterion](https://github.com/Snaipe/Criterion) for unit testing

---

## Installing MPICH Locally

To install MPICH into a project-local `.mpich/` directory without affecting your system:

```bash
./scripts/install_mpich.sh
```

This script will download, build, and install MPICH 4.3.0 into `.mpich/` in place where
it was executed.

---

## Building the Project

```bash
make build
```

Builds the final executable `ddc` under:

```
./build/bin/ddc
```

---

## Running the Classifier

```bash
make run MPI_FLAGS='-f nodes -n 8' RUN_FLAGS='input/ dict.txt out.txt'
```
- `MPI_FLAGS`: flags for `mpiexec` (default: '-f nodes -n 16')
- `RUN_FLAGS`: arguments for `ddc` (default: './example/input/ ./example/dict.txt ./example/output/result.txt')

---

## Running Tests

```bash
make test
```

Builds and runs all unit tests using the Criterion framework.

---

## Generating Documentation

```bash
make docs
```

- HTML docs: `docs/html/index.html`
- PDF docs: `docs/latex/refman.pdf`

---

## License

MIT License. See [LICENSE](LICENSE) for full details.

---

## References

- Michael J. Quinn, *Parallel Programming in C with MPI and OpenMP*, Chapter 9
- [MPICH](https://www.mpich.org/)
- [Doxygen](https://www.doxygen.nl/)
- [Criterion](https://github.com/Snaipe/Criterion)
