create database lab4;
\c lab4

create table safe_passwords (
  id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  input varchar(30) NOT NULL
);
INSERT INTO safe_passwords (input)
SELECT (
    SELECT string_agg(
        CASE WHEN random() > 0.5 THEN 'R' ELSE 'L' END || 
        to_char(floor(random() * 100), 'FM00'), 
        ''
    )
    FROM generate_series(1, 10) WHERE gs = gs
)
FROM generate_series(1, 1000000) AS gs;
-- INSERT INTO safe_passwords (input)
-- SELECT 
--     'L68L30R48L05R60L55L01L99R14L82'::varchar(30)
-- FROM generate_series(1, 1000000);
