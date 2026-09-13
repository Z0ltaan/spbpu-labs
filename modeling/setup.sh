#!/usr/bin/env bash

dir=$(dirname $0)
mkdir ${dir}/external
curl -O --output-dir ${dir}/external https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h
