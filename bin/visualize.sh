#!/bin/bash


set -eu
cd `dirname $0`/..

./third_party/tools/target/release/vis $@ > /dev/null

echo `readlink -f out.svg`
