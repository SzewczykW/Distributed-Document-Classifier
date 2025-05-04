#!/usr/bin/env bash

set -euo pipefail

# Check for required tools
for tool in curl make; do
    if ! command -v "${tool}" &> /dev/null; then
        echo "Error: Required tool '${tool}' is not installed or not in PATH."
        exit 1
    fi
done

# Configuration
MPICH_VERSION="4.3.0"
INSTALL_DIR="${MPICH_TARGET_DIR:-$(pwd)/.mpich}"
TARBALL="mpich-${MPICH_VERSION}.tar.gz"
SRC_DIR="mpich-${MPICH_VERSION}"
BUILD_DIR="${SRC_DIR}/build"
URL="https://www.mpich.org/static/downloads/${MPICH_VERSION}/${TARBALL}"

mkdir -p "${INSTALL_DIR}"
cd "${INSTALL_DIR}"
echo "Installing MPICH ${MPICH_VERSION} into: ${INSTALL_DIR}"

# Clean any previous state
rm -rf "$SRC_DIR" "$TARBALL"

# Download
echo "Downloading MPICH..."
curl -L -o "$TARBALL" "$URL"

# Extract
echo "Extracting..."
tar -xzf "$TARBALL"

# Configure and build
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring..."
../configure --prefix="$INSTALL_DIR" --disable-fortran

echo "Building..."
make -j"$(nproc)"

echo "Installing..."
make install

cd ..
rm -rf "$SRC_DIR" "$TARBALL"
cd ..

echo "MPICH ${MPICH_VERSION} installed successfully."
echo "Location: $(realpath "$INSTALL_DIR")"
echo ""
echo "To use this installation, export the following environment variable:"
echo "  export PATH=\${PATH}:\"$(realpath "${INSTALL_DIR}/build/bin")\""
echo "  export MPICH_TARGET_DIR=\"$(realpath "$INSTALL_DIR")\""

