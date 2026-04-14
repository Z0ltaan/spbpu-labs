#!/bin/sh

dbname="dblabs1"
file="dbdump.dump"

sudo -u postgres psql -f ./create_db.sql
sudo -u postgres psql $dbname < $file
