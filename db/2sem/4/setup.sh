#!/usr/bin/env bash

dir=$(dirname $0)

sudo -u postgres psql -f $dir/create_db.sql

sudo -u postgres psql -d lab4 -f $dir/get_password.sql -f $dir/analog.sql 
