EXPLAIN ANALYZE WITH password_lengths AS (
    SELECT
        get_safe_password_c(input) AS pwd,
        length(get_safe_password_c(input)) AS len
    FROM safe_passwords
),
max_len AS (
    SELECT MAX(len) FROM password_lengths
)
SELECT pwd, COUNT(*) 
FROM password_lengths 
WHERE len = (SELECT * FROM max_len)
GROUP BY pwd;

