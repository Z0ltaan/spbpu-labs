-- Развернуть массив ролей в плоский список (для аналитики)
SELECT firstname, lastname, jsonb_array_elements(rolesname->'roles') as role
FROM actors LIMIT 5;

