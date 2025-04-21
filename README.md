# Distributed Document Classifier

A parallel document classification engine based on MPI, inspired by Chapter 9 of *Parallel Programming in C with MPI and OpenMP* by Michael J. Quinn.

This project implements a scalable manager-worker architecture using MPICH to classify text documents into feature vectors. It uses hashing to match words against a shared dictionary, distributes processing across processes using MPI.

---

## Overview

- The **manager process** reads a directory of text documents and distributes file paths to worker processes.
- Each **worker process** tokenizes the document, hashes its content against a dictionary, and builds a feature vector.
- The vectors are returned to the manager, which writes the results to an output file.
- Supports `.txt`, `.html`, and `.tex` formats.

---

## Project Structure

```
.
├── include/             # Header files
├── src/                 # Core runtime: main, manager, worker, utils
├── core/                # Feature vector generation
├── lib/                 # Hash table, I/O and MPI utilities
├── tests/               # Criterion unit tests
├── docs/                # Doxygen config + generated docs
├── make/                # Makefile submodules (build, test, docs, run)
├── scripts/install_mpich.sh     # Script for downloading, building and installing MPICH locally
├── Makefile             # Entry point Makefile
```

There is also [CMakeLists.txt](CMakeLists.txt) here but it is not supported for now.

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

This script will download, build, and install MPICH 4.3.0 into `.mpich/`.

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
make run MPI_FLAGS="-np 4" RUN_FLAGS="--input ./data --output result.txt"
```

- `MPI_FLAGS`: flags for `mpirun` (e.g., `-np 4`)
- `RUN_FLAGS`: flags passed to the program:
  - `--input <folder>`: folder with `.txt`, `.html`, `.tex` files
  - `--output <file>`: output file for result vectors

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
