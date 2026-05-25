-- Развернуть массив ролей в плоский список (для аналитики)
SELECT first_name, last_name, jsonb_array_elements(roles->'roles') as role
FROM actors LIMIT 5;

