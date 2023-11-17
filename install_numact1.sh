#!/bin/bash

set -eu -o pipefail

NUMACTL_VERSION="2.0.16"

TARGET="numact1"

INSTALL_DIR="$HOME/$TARGET"
INSTALL_PATH="$PATH"

read -p "This script will install numactl version $NUMACTL_VERSION in $INSTALL_DIR. Continue? (y/n): " choice

if [[ $choice =~ ^[Yy]$ ]]; then
    TEMP_DIR=$(mktemp -d)

    cd "$TEMP_DIR" || exit

    wget https://github.com/numactl/numactl/releases/download/v${NUMACTL_VERSION}/numactl-${NUMACTL_VERSION}.tar.gz

    tar -xf numactl-${NUMACTL_VERSION}.tar.gz

    cd numactl-${NUMACTL_VERSION} || exit

    ./configure --prefix="$INSTALL_DIR"
    make && make install

    #./configure --prefix="$INSTALL_PATH"
    #make && make install

    cd "$HOME" || exit
    rm -rf "$TEMP_DIR"
    echo -e
    echo "------------------------------------------------------------------------------"
    echo "Numactl version ${NUMACTL_VERSION} has been installed in $INSTALL_DIR and PATH"
    echo "------------------------------------------------------------------------------"
else
    echo "Installation failed."
fi