#!/bin/bash

PROJECT_ROOT=$(realpath $(dirname `realpath $0`)/..)
BUILD_PATH=$PROJECT_ROOT/build
INSTALL_PATH=$BUILD_PATH/install

# Parse the received commands
while :; do
    case $1 in
        -c|--clean) FLAG_CLEAN="SET"
        ;;
        *) break
    esac
    shift
done

# Cleanup the existing files
if [[ ! -z "$FLAG_CLEAN" ]]; then
    rm -rf $BUILD_PATH
fi

# Compile!
mkdir -p $BUILD_PATH
cd $BUILD_PATH
cmake -DCMAKE_BUILD_TYPE=Release -H$PROJECT_ROOT
cmake --build . -j7

mkdir -p $INSTALL_PATH
cmake --install . --prefix $INSTALL_PATH