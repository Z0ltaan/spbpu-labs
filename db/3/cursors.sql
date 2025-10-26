CREATE OR REPLACE PROCEDURE calculate_master_bonus(
    IN masterid INTEGER,
    IN base_salary NUMERIC(10,2),
    INOUT total NUMERIC(10,2) DEFAULT 0
)
LANGUAGE plpgsql
AS $$
DECLARE
    car_record RECORD;
    works_count INTEGER;
    car_bonus NUMERIC(10,2) := 0;
    total_bonus NUMERIC(10,2) := 0;

    car_cursor CURSOR FOR
    WITH current_month as (SELECT DATE_TRUNC('month', CURRENT_DATE) AS value), m AS ( SELECT masterid AS mid)
        SELECT 
            w.car_id,
            COUNT(*) as works_count
        FROM works w
        WHERE w.master_id = (select mid from m) AND w.date_work >= ( SELECT value from current_month) - INTERVAL '1 month'
        GROUP BY w.car_id
        ORDER BY w.car_id;

BEGIN
    IF NOT EXISTS (SELECT 1 FROM masters WHERE id = masterid) THEN
        RAISE EXCEPTION 'Master % does not exist', masterid;
    END IF;

    OPEN car_cursor;

    LOOP
        FETCH car_cursor INTO car_record;

        EXIT WHEN NOT FOUND;

        IF car_record.works_count < 3 THEN
            car_bonus := car_bonus + base_salary * 0.05;
        ELSE
            car_bonus := car_bonus + car_record.works_count * base_salary * 0.07;
        END IF;

        RAISE INFO 'Car: %, работ: %, текущая премия: %',
            car_record.car_id, car_record.works_count, car_bonus;
    END LOOP;

    CLOSE car_cursor;

    total := base_salary + car_bonus;
    RAISE NOTICE 'Master: %, total income: %', masterid, total;
END;
$$;
