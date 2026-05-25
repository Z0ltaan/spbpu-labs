-- Найти актеров, у которых есть роль в фильме "Unforgiven"
SELECT first_name, last_name FROM actors 
WHERE roles @> '{"roles": [{"title": "Unforgiven"}]}';
