EXPLAIN (ANALYZE, VERBOSE, COSTS, TIMING, BUFFERS)
SELECT id, comment, created_at
FROM child_table
WHERE to_tsvector('english', comment) @@ to_tsquery('english', 'mos');
