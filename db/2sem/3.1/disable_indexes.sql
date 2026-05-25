DROP INDEX IF EXISTS idx_child_id_created;
DROP INDEX IF EXISTS idx_child_main_id_created;
DROP INDEX IF EXISTS idx_child_fts;

SET enable_seqscan = ON;
SET enable_indexscan = OFF;
SET enable_indexonlyscan = OFF;

ANALYZE child_table;
