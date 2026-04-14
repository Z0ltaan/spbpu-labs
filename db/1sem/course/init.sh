#!/bin/sh

user="dblabs1"
db="$user"
sql_dir="./sql"
triggers_dir="$sql_dir/triggers"
procedures_dir="$sql_dir/procedures"
functions_dir="$sql_dir/functions"

sudo -u postgres psql -f $sql_dir/create_db.sql

sudo -u $user psql -f $sql_dir/create_tables.sql
sudo -u $user psql -f $sql_dir/set_connections.sql

sudo -u $user psql -f $triggers_dir/forbid_more_than_ten_masters.sql -f $triggers_dir/forbid_non_unique.sql -f $triggers_dir/forbid_overwork.sql -f $functions_dir/hash_string.sql

sudo -u $user psql -f $procedures_dir/get_score_table.sql -f $procedures_dir/sum_up_costs.sql -f $procedures_dir/register_user.sql
