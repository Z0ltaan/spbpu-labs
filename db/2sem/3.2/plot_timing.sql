EXPLAIN (ANALYZE, TIMING) 
SELECT rolesname->'roles'->0->>'year' FROM actors WHERE id = <ID>;
