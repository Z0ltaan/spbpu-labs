INSERT INTO services (name, cost_our, cost_foreign) VALUES ('someservice', 20000, 20000);
INSERT INTO works (date_work, master_id, car_id, service_id) VALUES ('2024-01-15', 1, 1, (SELECT id FROM services WHERE name='someservice'));

BEGIN;
  INSERT INTO cars (num, color, mark, is_foreign) VALUES ('fffff', 'color2', 'mark2', false);
  INSERT INTO services (name, cost_our, cost_foreign) VALUES ('someservice2', 100, 20);
  INSERT INTO works (date_work, master_id, car_id, service_id) VALUES ('2025-01-15', 1, (SELECT id FROM cars WHERE num='fffff'), (SELECT id FROM services WHERE name='someservice2'));
COMMIT;
