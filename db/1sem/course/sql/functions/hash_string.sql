CREATE OR REPLACE FUNCTION hash_string(string text)
RETURNS CHAR(256)
LANGUAGE plpgsql
AS $$
BEGIN
  RETURN encode(digest(string, 'sha256'), 'hex');
END;
$$;
