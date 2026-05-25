CREATE INDEX idx_child_id_created ON child_table(id, created_at);

CREATE INDEX idx_child_main_id_created ON child_table(main_id, created_at);

CREATE INDEX idx_child_fts ON child_table USING GIN (to_tsvector('english', comment));

SET enable_seqscan = OFF;
SET enable_indexscan = ON;
SET enable_indexonlyscan = ON;

ANALYZE child_table;
