CREATE OR REPLACE FUNCTION check_if_num_is_unique() RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
  IF (EXISTS(SELECT 1 FROM cars c WHERE c.num = NEW.num)) THEN
    RAISE EXCEPTION 'Car with number % already exists in the table', NEW.num;
  END IF;

  RETURN NEW;
END;
$$;

CREATE OR REPLACE TRIGGER forbid_non_unique_car_numbers BEFORE INSERT ON cars FOR EACH ROW EXECUTE FUNCTION check_if_num_is_unique();

