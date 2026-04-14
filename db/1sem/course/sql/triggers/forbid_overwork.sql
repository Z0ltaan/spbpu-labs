CREATE OR REPLACE FUNCTION has_worked_that_day() RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
DECLARE
  max_work_count INTEGER := 1;
BEGIN
  IF ((SELECT COUNT(*) FROM WORKS w WHERE w.master_id = NEW.master_id AND DATE_TRUNC('day', w.date_work::timestamp) = DATE_TRUNC('day',NEW.date_work::timestamp)) > max_work_count) THEN
    RAISE EXCEPTION 'Already too worked up; He did % or more...', max_work_count;
  END IF;
  RETURN NEW;
END;
$$;

CREATE OR REPLACE TRIGGER forbid_more_than_works_in_day BEFORE INSERT ON works FOR EACH ROW EXECUTE FUNCTION has_worked_that_day();
