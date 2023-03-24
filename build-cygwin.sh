#!/bin/bash -x

SCRIPT=`basename $0 .sh`
SCRIPT_DIR=$(realpath $(dirname $0))
pushd $SCRIPT_DIR

echo "################### BUILDING $0 ####################"
    
rm -rf .build || exit 1
mkdir -p .build || exit 1
cd .build

cmake --install-prefix=$CMAKE_INSTALL_PREFIX -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DCMAKE_C_FLAGS="$CMAKE_C_FLAGS" \
    -DCMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS" \
    -DCMAKE_STATIC_LINKER_FLAGS=-v --debug-trycompile -G 'Unix Makefiles' .. || exit 1
make SHELL='/bin/bash -x' || exit 1
make install SHELL='/bin/bash -x' || exit 1

popd
