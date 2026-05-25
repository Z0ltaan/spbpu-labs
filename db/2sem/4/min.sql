EXPLAIN ANALYZE WITH ranked_passwords AS (
    SELECT 
        id,
        get_safe_password_c(input) AS pwd
    FROM safe_passwords
),
valid_passwords AS (
    SELECT 
        id, 
        LEAD(id) OVER (ORDER BY id) AS next_id
    FROM ranked_passwords
    WHERE pwd <> ''
)
SELECT MIN(next_id - id - 1) AS min_distance 
FROM valid_passwords 
WHERE next_id IS NOT NULL;

