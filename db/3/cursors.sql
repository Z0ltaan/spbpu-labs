-- CREATE
-- OR REPLACE PROCEDURE calculate_bonus(master INTEGER, base INTEGER, INOUT bonus INTEGER DEFAULT 0)
-- LANGUAGE plpgsql
-- AS $$
-- DECLARE
-- service_count INTEGER;
-- BEGIN
--
-- END;
-- $$;



-- ii
CREATE OR REPLACE PROCEDURE calculate_master_bonus(
    IN p_master_id INTEGER,
    IN p_base_salary NUMERIC(10,2),
    INOUT p_total_income NUMERIC(10,2) DEFAULT 0
)
LANGUAGE plpgsql
AS $$
DECLARE
    car_record RECORD;
    works_count INTEGER;
    car_bonus NUMERIC(10,2);
    total_bonus NUMERIC(10,2) := 0;
    
    -- Курсор по автомобилям с количеством работ
    car_cursor CURSOR FOR
        WITH current_month as (SELECT DATE_TRUNC('month', CURRENT_DATE) AS value)
        SELECT 
            w.car_id,
            COUNT(*) as works_count
        FROM works w
        WHERE w.master_id = p_master_id
          AND w.date_work >= current_month.value
          AND w.date_work < current_month.value+ INTERVAL '1 month'
        GROUP BY w.car_id
        ORDER BY w.car_id;
    
BEGIN
    IF NOT EXISTS (SELECT 1 FROM masters WHERE id = p_master_id) THEN
        RAISE EXCEPTION 'Master % does not exist', p_master_id;
    END IF;
    
    OPEN car_cursor;
    
    LOOP
        FETCH car_cursor INTO car_record;
        
        EXIT WHEN NOT FOUND;
        
        IF car_record.works_count < 3 THEN
            car_bonus := p_base_salary * 0.05; -- 5%
        ELSE
            car_bonus := p_base_salary * 0.07; -- 7%
        END IF;
        
        total_bonus := total_bonus + car_bonus;
        
        RAISE INFO 'Car: %, работ: %, премия: %', 
            car_record.car_id, car_record.works_count, car_bonus;
    END LOOP;
    
    CLOSE car_cursor;
    
    p_total_income := p_base_salary + total_bonus;
    RAISE NOTICE 'Master: %, total income: %', p_master_id, p_total_income;
END;
$$;
