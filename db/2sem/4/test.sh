#!/usr/bin/env bash

dir=$(dirname $0)

sudo -u postgres psql -d lab4 -f $dir/performance_test.sql -f $dir/longest.sql -f $dir/min.sql
