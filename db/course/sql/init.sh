#!/bin/sh

user="dblabs1"
db="$user"
sql_dir="."
triggers_dir="$sql_dir/triggers"
procedures_dir="$sql_dir/procedures"

sudo -u postgres psql -f $sql_dir/create_db.sql

sudo -u $user psql -f $sql_dir/create_tables.sql
sudo -u $user psql -f $sql_dir/set_connections.sql

sudo -u $user psql -f $triggers_dir/forbid_more_than_ten_masters.sql -f $triggers_dir/forbid_non_unique.sql -f $triggers_dir/forbid_overwork.sql

sudo -u $user psql -f $procedures_dir/get_scores_table.sql -f $procedures_dir/sum_up_costs.sql
