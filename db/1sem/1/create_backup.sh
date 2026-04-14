#!/bin/sh

dbname="dblabs1"
file="db.dump"
sudo -u postgres pg_dump $dbname > $file
