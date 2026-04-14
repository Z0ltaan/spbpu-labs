#!/usr/bin/env bash

bin_dir=$(dirname "$0")
query_dir=$bin_dir/queries

psql_options="-h localhost -p 5432 -U postgres"

PGPASSWORD=a psql $psql_options -f $bin_dir/create_db.sql
PGPASSWORD=a psql $psql_options -d wording -f $bin_dir/generator.sql -f $bin_dir/generate_values.sql
PGPASSWORD=a psql $psql_options -d wording -f $bin_dir/constraints.sql
