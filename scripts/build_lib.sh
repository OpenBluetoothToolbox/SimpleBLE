#!/bin/bash

# Some portions of this file where inspired from:
#   https://medium.com/@Drew_Stokes/bash-argument-parsing-54f3b81a6a8f

# If the current script is running in MacOS, print a warning
if [[ "$OSTYPE" == "darwin"* ]]; then

    # Define the realpath function, as MacOs doesn't have it
    realpath() {
        OURPWD=$PWD
        cd "$(dirname "$1")"
        LINK=$(readlink "$(basename "$1")")
        while [ "$LINK" ]; do
            cd "$(dirname "$LINK")"
            LINK=$(readlink "$(basename "$1")")
        done
        REALPATH="$PWD/$(basename "$1")"
        cd "$OURPWD"
        echo "$REALPATH"
    }

fi

# Parse incoming arguments
PARAMS=""
while (( "$#" )); do
  case "$1" in
    -c|--clean)
      FLAG_CLEAN=0
      shift
      ;;
    -t|--test)
      FLAG_TEST=0
      shift
      ;;
    -*|--*=) # unsupported flags
      echo "Error: Unsupported flag $1" >&2
      exit 1
      ;;
    *) # preserve positional arguments
      PARAMS="$PARAMS $1"
      shift
      ;;
  esac
done

# Set positional arguments in their proper place
eval set -- "$PARAMS"

# Validate that there is at least one positional argument
if [ -z "$1" ]; then
    echo "Error: No library specified" >&2
    exit 1
fi
LIB_NAME=$1

PROJECT_ROOT=$(realpath $(dirname `realpath $0`)/..)
SOURCE_PATH=$PROJECT_ROOT/$LIB_NAME
BUILD_PATH=$PROJECT_ROOT/build_$LIB_NAME

# If FLAG_TEST is set, build the library with the test argument
if [[ ! -z "$FLAG_TEST" ]]; then
    BUILD_TEST_ARG="-D${LIB_NAME^^}_TEST=ON"
fi

# If FLAG_CLEAN is set, clean the build directory
if [[ ! -z "$FLAG_CLEAN" ]]; then
    rm -rf $BUILD_PATH
fi

cmake -H$SOURCE_PATH -B $BUILD_PATH $BUILD_TEST_ARG
cmake --build $BUILD_PATH -j7


INSTALL_PATH=$BUILD_PATH/install