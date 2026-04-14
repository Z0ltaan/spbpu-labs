-- Индекс для запроса А (составной)
CREATE INDEX idx_child_id_created ON child_table(id, created_at);

-- Индекс для запроса Б
CREATE INDEX idx_child_main_id_created ON child_table(main_id, created_at);
CREATE INDEX idx_main_id ON main_table(id);  -- если нет PK

-- Индекс для полнотекстового поиска (запрос В)
CREATE INDEX idx_child_fts ON child_table USING GIN (to_tsvector('english', comment));

-- Дополнительный индекс для ускорения удаления старых данных (партиционирование)
-- Создаём партиции по месяцам
-- CREATE TABLE child_table_partitioned (
--     LIKE child_table INCLUDING ALL
-- ) PARTITION BY RANGE (created_at);
--
-- -- Создаём партиции
-- CREATE TABLE child_table_2024_01 PARTITION OF child_table_partitioned
-- FOR VALUES FROM ('2024-01-01') TO ('2024-02-01');
--
-- CREATE TABLE child_table_2024_02 PARTITION OF child_table_partitioned
-- FOR VALUES FROM ('2024-02-01') TO ('2024-03-01');
-- ... и так далее;
SET enable_seqscan = OFF;
SET enable_indexscan = ON;
SET enable_indexonlyscan = ON;
