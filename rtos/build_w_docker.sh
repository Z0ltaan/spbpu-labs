#!/usr/bin/env bash

dir=$(dirname $0)

docker build --output=${dir}/build ${dir}
