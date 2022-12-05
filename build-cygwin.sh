#!/bin/bash -x

cd $(dirname $0)

export BUILD_SHARED_LIBS=OFF
export CMAKE_INSTALL_PREFIX=$PWD/.install
export CMAKE_C_FLAGS=-fpermissive
export CMAKE_CXX_FLAGS=-fpermissive

rm -rf .build .install
mkdir -p .build
cd .build

cmake -G 'Unix Makefiles' -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX \
                          -DCMAKE_C_FLAGS=$CMAKE_C_FLAGS -DCMAKE_CXX_FLAGS=$CMAKE_CXX_FLAGS ..
make
make install
