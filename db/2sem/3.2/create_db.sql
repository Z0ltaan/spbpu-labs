CREATE DATABASE imdb_test;
\c imdb_test;

CREATE TABLE actors (
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(200),
    last_name VARCHAR(200),
    roles JSONB
);

-- Создание индекса для JSONB полей
CREATE INDEX idx_actors_roles ON actors USING GIN (roles);

