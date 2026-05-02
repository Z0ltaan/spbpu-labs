-- Найти всех актеров, снимавшихся в 1994 году (через jsonpath)
SELECT firstname, lastname FROM actors 
WHERE rolesname @@ '$.roles[*].year == "1994"';
