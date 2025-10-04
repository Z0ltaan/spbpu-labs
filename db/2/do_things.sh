#!/bin/sh

sudo -u dblabs1 psql -f do_select.sql
sudo -u dblabs1 psql -f do_insert.sql
sudo -u dblabs1 psql -f do_delete.sql
sudo -u dblabs1 psql -f do_modify.sql
