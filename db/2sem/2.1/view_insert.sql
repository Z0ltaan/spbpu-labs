CREATE OR REPLACE FUNCTION insert_into_services_cap()
RETURNS TRIGGER AS $$
DECLARE
    v_service_id INT;
BEGIN
    INSERT INTO services (name, cost_our, cost_foreign)
    VALUES (NEW.name, 5000, 12000)
    RETURNING id INTO v_service_id;    INSERT INTO works (date_work,car_id, service_id, master_id)

    VALUES (CURRENT_DATE, NEW.car_id, NEW.id, 2);
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER tr_insert_services_cap
INSTEAD OF INSERT ON services_cap
FOR EACH ROW EXECUTE FUNCTION insert_into_services_cap();
