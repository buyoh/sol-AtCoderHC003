#!/bin/bash

set -eu
cd `dirname $0`/..

mkdir -p out/cases/

# for i in `seq 30`; do
#   bin/generator.sh > out/cases/in_${i}.txt
# done

pushd third_party/tools > /dev/null
cargo run --release --bin gen seeds.txt
cp in/*.txt ../../out/cases/
popd > /dev/null
