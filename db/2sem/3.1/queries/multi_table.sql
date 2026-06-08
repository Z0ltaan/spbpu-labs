EXPLAIN (ANALYZE, COSTS, TIMING, BUFFERS)
SELECT m.id, m.some_field, c.comment, c.created_at
FROM main_table m
JOIN child_table c ON m.id = c.main_id
WHERE m.id BETWEEN 10001 AND 20000
  AND c.created_at >= '2024-01-15';
