INSERT INTO services_cap (id, car_id) VALUES (1, 1); 

SELECT * FROM works WHERE car_id = 1 AND service_id = 1;
SELECT * FROM services_cap WHERE car_id = 1;

