SELECT * FROM cars LIMIT 5;
INSERT INTO cars (mark, color, num, is_foreign) VALUES ('TestMark', 'Red', 'gooooaal', TRUE);

SELECT name, cost_our, cost_foreign FROM services LIMIT 5;
SELECT id FROM services;
UPDATE services SET cost_our = cost_our + 100 WHERE name = 'Масло';
