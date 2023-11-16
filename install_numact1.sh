#!/bin/bash

set -eu -o pipefail # fail on error and report it, debug all lines

# Set the version of numactl to install
NUMACTL_VERSION="2.0.16"

# Set the installation directory in your home directory
INSTALL_DIR="$HOME/nlzl16"

read -p "This script will install numactl version $NUMACTL_VERSION in $INSTALL_DIR. Continue? (y/n): " choice

# Check the user's choice
if [[ $choice =~ ^[Yy]$ ]]; then
    # Create a temporary directory for the build
    TEMP_DIR=$(mktemp -d)

    # Navigate to the temporary directory
    cd "$TEMP_DIR" || exit

    # Download the numactl source tarball
    wget https://github.com/numactl/numactl/releases/download/v${NUMACTL_VERSION}/numactl-${NUMACTL_VERSION}.tar.gz

    # Extract the tarball
    tar -xf numactl-${NUMACTL_VERSION}.tar.gz

    # Navigate into the numactl source directory
    cd numactl-${NUMACTL_VERSION} || exit

    # Configure the build to install in the specified directory
    ./configure --prefix="$INSTALL_DIR"

    # Build and install numactl
    make && make install

    # Clean up temporary files
    cd "home2/nlzl16" || exit
    rm -rf "$TEMP_DIR"

    echo "Numactl version ${NUMACTL_VERSION} has been installed in $INSTALL_DIR"
    echo "Make sure to add $INSTALL_DIR/bin to your PATH environment variable."
else
    echo "Installation failed."
fi