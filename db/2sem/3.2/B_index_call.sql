-- Извлечь название первой роли для каждого актера
SELECT rolesname->'roles'->0->>'title' FROM actors LIMIT 10;
