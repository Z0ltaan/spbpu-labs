#!/bin/sh

dbname="dblabs1"

sudo -u $dbname psql -f do_select.sql
sudo -u $dbname psql -f do_insert.sql
sudo -u $dbname psql -f do_delete.sql
sudo -u $dbname psql -f do_modify.sql
