CREATE EXTENSION IF NOT EXISTS pgcrypto;

CREATE
OR REPLACE PROCEDURE register_user (ulogin text , upassword text, urole text)
LANGUAGE sql
AS $$
  insert into users (login, password, role) values (hash_string(ulogin), hash_string(upassword), urole);

$$;
