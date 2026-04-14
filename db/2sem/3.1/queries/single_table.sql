EXPLAIN (ANALYZE, VERBOSE, COSTS, TIMING, BUFFERS, FORMAT JSON)
SELECT id, comment, created_at 
FROM child_table 
WHERE id BETWEEN 5000000 AND 5001000
  AND created_at >= '2024-01-01'
  AND created_at < '2024-02-01'
  AND LENGTH(comment) > 50;
