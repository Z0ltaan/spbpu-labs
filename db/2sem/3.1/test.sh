#!/usr/bin/env bash

bin_dir=$(dirname $0)
queries_dir=$bin_dir/queries
output_dir=$bin_dir/out

psql_options="-h localhost -p 5432 -U postgres -d wording"

PGPASSWORD=a psql $psql_options -f $queries_dir/single_table.sql > $output_dir/single.txt
PGPASSWORD=a psql $psql_options -f $queries_dir/multi_table.sql > $output_dir/multi.txt
PGPASSWORD=a psql $psql_options -f $queries_dir/full_text.sql > $output_dir/full.txt

PGPASSWORD=a psql $psql_options -f $bin_dir/indexes.sql

PGPASSWORD=a psql $psql_options -f $queries_dir/single_table.sql > $output_dir/single_idx.txt
PGPASSWORD=a psql $psql_options -f $queries_dir/multi_table.sql > $output_dir/multi_idx.txt
PGPASSWORD=a psql $psql_options -f $queries_dir/full_text.sql > $output_dir/full_idx.txt

PGPASSWORD=a psql $psql_options -f $bin_dir/disable_indexes.sql
