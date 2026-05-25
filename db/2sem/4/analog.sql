CREATE OR REPLACE FUNCTION get_safe_password_pl(input_str varchar) 
RETURNS varchar AS $$
DECLARE
    current_pos int := 50;
    result varchar := '';
    i int;
    direction char(1);
    val_str varchar(2);
    val_int int;
BEGIN
    FOR i IN 1..length(input_str) BY 3 LOOP
        direction := substr(input_str, i, 1);
        val_str := substr(input_str, i + 1, 2);
        val_int := val_str::int;
        
        IF direction = 'R' THEN
            current_pos := (current_pos + val_int) % 100;
        ELSE
            current_pos := (current_pos - val_int + 100) % 100;
        END IF;
        
        IF current_pos = 0 THEN
            result := result || val_str;
        END IF;
    END LOOP;
    RETURN result;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

