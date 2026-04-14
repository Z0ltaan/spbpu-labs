ALTER TABLE main_table ADD PRIMARY KEY (id);
ALTER TABLE child_table ADD PRIMARY KEY (id);
ALTER TABLE child_table ADD FOREIGN KEY (main_id) REFERENCES main_table(id);
-- CREATE INDEX ON child_table(main_id);
