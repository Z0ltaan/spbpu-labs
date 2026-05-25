-- Извлечь название первой роли для каждого актера
SELECT roles->'roles'->0->>'title' FROM actors LIMIT 10;
