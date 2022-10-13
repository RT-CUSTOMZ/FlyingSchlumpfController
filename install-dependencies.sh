#!/bin/sh

set -x

if [ ! -d "boost" ]; then
wget https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.tar.bz2
tar -xf boost_1_80_0.tar.bz2
rm boost_1_80_0.tar.bz2
mv boost_1_80_0 boost
fi