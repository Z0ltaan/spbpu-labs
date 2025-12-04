ALTER TABLE works 
ADD CONSTRAINT fk_works_cars 
FOREIGN KEY (car_id) 
REFERENCES cars(id) 
ON DELETE CASCADE;

ALTER TABLE works 
ADD CONSTRAINT fk_works_masters 
FOREIGN KEY (master_id) 
REFERENCES masters(id) 
ON DELETE CASCADE;

ALTER TABLE works 
ADD CONSTRAINT fk_works_services 
FOREIGN KEY (service_id) 
REFERENCES services(id) 
ON DELETE CASCADE;
