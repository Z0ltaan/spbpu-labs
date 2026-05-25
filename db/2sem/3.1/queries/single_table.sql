EXPLAIN (ANALYZE, COSTS, TIMING, BUFFERS)
SELECT id, comment, created_at 
FROM child_table 
WHERE id > 5000000
  AND created_at >= '2024-01-01'
