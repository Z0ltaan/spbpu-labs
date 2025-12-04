CREATE OR REPLACE FUNCTION check_if_more_than_ten_masters() RETURNS TRIGGER LANGUAGE plpgsql
AS $$
DECLARE
max_master_count SMALLINT := 10;
BEGIN
  IF ((SELECT COUNT(*) FROM masters m) >= max_master_count) THEN
    RAISE EXCEPTION 'Already % masters present', max_master_count;
  END IF;

  RETURN NEW;
END;
$$;

CREATE OR REPLACE TRIGGER forbid_more_than_ten_masters BEFORE INSERT ON masters FOR EACH STATEMENT EXECUTE FUNCTION check_if_more_than_ten_masters();


