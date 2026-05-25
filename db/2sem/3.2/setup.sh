#!/usr/bin/env bash

script_dir=$(dirname $0)

PGPASSWORD=a psql -U postgres -h localhost -f $script_dir/create_db.sql

PGPASSWORD=a psql -U postgres -h localhost -d imdb_test -f $script_dir/parser.sql -f $script_dir/parse_file.sql
