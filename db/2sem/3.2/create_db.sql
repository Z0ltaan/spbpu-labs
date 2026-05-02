-- Создание базы данных
CREATE DATABASE IMDB_test;
\c IMDB_test;

-- Создание таблицы актеров
CREATE TABLE actors (
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(100),
    last_name VARCHAR(100),
    roles JSONB,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Создание индекса для JSONB полей
CREATE INDEX idx_actors_roles ON actors USING GIN (roles);
