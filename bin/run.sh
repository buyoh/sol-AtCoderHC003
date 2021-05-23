#!/bin/bash

set -eu
cd `dirname $0`/..

if [[ -n ${1+x} ]]; then
  FILE_IN=$1
else
  FILE_IN=third_party/tools/in/0000.txt
fi

FILE_IN=`readlink -f $FILE_IN`

cd third_party/tools

cargo run --release --bin tester $FILE_IN ../../out/main
