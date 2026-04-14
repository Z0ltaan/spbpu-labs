EXPLAIN (ANALYZE, VERBOSE, COSTS, TIMING, BUFFERS, FORMAT JSON)
SELECT m.id, m.some_field, c.comment, c.created_at
FROM main_table m
JOIN child_table c ON m.id = c.main_id
WHERE m.id BETWEEN 10000 AND 20000
  AND c.created_at >= '2024-01-15'
  AND c.created_at < '2024-01-20'
  AND c.comment LIKE 'mos';
