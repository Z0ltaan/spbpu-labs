-- Удалить индексы, если они существуют
DROP INDEX IF EXISTS idx_child_id_created;
DROP INDEX IF EXISTS idx_child_main_id_created;
DROP INDEX IF EXISTS idx_child_fts;

-- ИЛИ временно отключить последовательное сканирование (принудительно)
SET enable_seqscan = ON;
SET enable_indexscan = OFF;
SET enable_indexonlyscan = OFF;
