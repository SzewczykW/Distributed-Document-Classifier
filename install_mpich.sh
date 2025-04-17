#!/usr/bin/env bash

set -euo

MPICH_VERSION="4.3.0"
TARGET_DIR="$(pwd -P)/.mpich"
ARCHIVE="mpich-${MPICH_VERSION}.tar.gz"
SOURCE_DIR="mpich-${MPICH_VERSION}"
INSTALL_DIR="${TARGET_DIR}/${SOURCE_DIR}/build"
URL="https://www.mpich.org/static/downloads/${MPICH_VERSION}/${ARCHIVE}"

echo "Downloading MPICH ${MPICH_VERSION}..."
mkdir -p "${TARGET_DIR}"
cd "${TARGET_DIR}"

rm -rf "${SOURCE_DIR}"
rm -rf "build"
rm -rf "${INSTALL_DIR}"

wget -O "${ARCHIVE}" "${URL}"

echo "Extracting archive..."
tar -xzf "${ARCHIVE}"

echo "Building..."
cd "${SOURCE_DIR}"
./configure --prefix="${INSTALL_DIR}" --disable-fortran
make -j"$(nproc)"
make install

echo "Done!"
echo "MPICH installed at: ${INSTALL_DIR}"
