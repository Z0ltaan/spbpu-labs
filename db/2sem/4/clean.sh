#!/usr/bin/env bash

dir=$(dirname $0)

sudo -u postgres psql -f $dir/drop_db.sql
