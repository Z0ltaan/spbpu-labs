-- Найти всех актеров, снимавшихся в 1994 году (через jsonpath)
SELECT first_name, last_name FROM actors 
WHERE roles @@ '$.roles[*].year == "1994"';
