#!/usr/bin/env bash

script_dir=$(dirname $0)

PGPASSWORD=a psql -U postgres -h localhost -f $script_dir/drop_db.sql
