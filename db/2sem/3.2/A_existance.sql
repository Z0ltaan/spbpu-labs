-- Найти актеров, у которых есть роль в фильме "Unforgiven"
SELECT firstname, lastname FROM actors 
WHERE rolesname @> '{"roles": [{"title": "Unforgiven"}]}';

-- Проверить, есть ли у актера хотя бы одна роль с указанием номера в титрах
SELECT count(*) FROM actors WHERE rolesname->'roles'->0 ? 'credit';

