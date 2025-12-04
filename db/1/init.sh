#!/bin/sh

sudo -u dblabs1 psql -f ./create_tables.sql
sudo -u dblabs1 psql -f ./set_connections.sql
# sudo -u dblabs1 psql -f ./insert_deafult.sql

