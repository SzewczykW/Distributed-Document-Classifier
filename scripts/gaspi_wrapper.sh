#!/bin/sh
# 20/05/2024 FIS - wrapper to pass env var to remote nodes
# workaround of a bug in GASPI 1.5.1

export CUDA_TARGET_DIR="/opt/nfs/cuda-12.1"
export MPICH_TARGET_DIR="/opt/nfs/mpich-4.3.0"
export GASPI_TARGET_DIR="/opt/nfs/gpi2-1.5.1"

export LD_LIBRARY_PATH="${GASPI_TARGET_DIR}/lib64:${CUDA_TARGET_DIR}/lib64:${MPICH_TARGET_DIR}/lib:${LD_LIBRARY_PATH}"

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)

cd "$SCRIPT_DIR/.."

exec "$@"
