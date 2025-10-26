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

-- kkk
CREATE OR REPLACE FUNCTION check_if_date_isnt_within_day() RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
  IF (AGE(OLD.date_work, NEW.date_work) > INTERVAL'1 day' ) THEN
    RAISE EXCEPTION 'Work date change is not within a day when compared to previous date';
  END IF;
  RETURN NEW;
END;
$$;

CREATE OR REPLACE TRIGGER forbid_more_than_day_date_change BEFORE UPDATE ON works FOR EACH ROW EXECUTE FUNCTION check_if_date_isnt_within_day();

--kk
CREATE OR REPLACE FUNCTION prevent_car_deletion_with_works()
RETURNS TRIGGER 
LANGUAGE plpgsql
AS $$
DECLARE
    works_count INTEGER;
BEGIN
    SELECT COUNT(*) INTO works_count
    FROM works 
    WHERE car_id = OLD.id;

    IF works_count > 0 THEN
        RAISE EXCEPTION 'Cant delete car % because there is/are % work(s)', 
            OLD.id, works_count;
    END IF;
    RETURN OLD;
END;
$$;

CREATE TRIGGER prevent_car_deletion
    BEFORE DELETE ON cars
    FOR EACH ROW
    EXECUTE FUNCTION prevent_car_deletion_with_works();
