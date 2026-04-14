#!/usr/bin/env bash

bin_dir=$(dirname $0)

psql_options="-h localhost -p 5432 -U postgres"

PGPASSWORD=a psql $psql_options -f $bin_dir/drop_db.sql
