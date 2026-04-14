CREATE DATABASE wording;
\c wording

CREATE TABLE main_table (
    id         BIGINT,
    some_field TEXT,
    created_at TIMESTAMP
);

CREATE TABLE child_table (
    id         BIGINT,
    main_id    BIGINT,
    comment    TEXT,
    created_at TIMESTAMP
);

CREATE TABLE lookup (
    id   INT,
    name TEXT
);
