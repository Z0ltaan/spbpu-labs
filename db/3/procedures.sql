CREATE
OR REPLACE PROCEDURE get_average_cost () LANGUAGE plpgsql AS $$ 
DECLARE
car_instance RECORD;
BEGIN 

RAISE INFO 'Average car service cost';

FOR car_instance IN (
SELECT
  c.id,
  c.num,
  COALESCE(
    AVG(
      CASE
        WHEN c.is_foreign THEN s.cost_foreign
        ELSE s.cost_our
      END
    ),
    0
  ) AS average_cost
FROM
  cars c
  LEFT JOIN works w ON (c.id = w.car_id)
  LEFT JOIN services s ON (w.service_id = s.id)
GROUP BY
  c.id,
  c.num
)
LOOP
  RAISE INFO 'id: %, number: %, average_cost: %', car_instance.id, car_instance.num, car_instance.average_cost;
END LOOP;

END;
$$;


CREATE OR REPLACE PROCEDURE get_service_cost_and_number(carid INTEGER, serviceid INTEGER)
LANGUAGE plpgsql AS $$
DECLARE
  total_cost INTEGER := 0;
  works_count integer := 0;
BEGIN
  SELECT 
        COALESCE(SUM(
            CASE 
                WHEN c.is_foreign THEN s.cost_foreign 
                ELSE s.cost_our 
            END
        ), 0), 
        COUNT(w.id) 
    INTO total_cost, works_count
    FROM works w
    JOIN services s ON w.service_id = s.id
    JOIN cars c ON w.car_id = c.id
    WHERE s.id = serviceid
      AND c.id = carid;
    
    -- Выводим результат
    RAISE INFO 'Услуга: "%" для автомобиля "%"', serviceid, carid;
    RAISE INFO 'Общая стоимость за все время: %', total_cost;
    RAISE INFO 'Количество проведенных работ: %', works_count;
END;
$$;

CREATE OR REPLACE PROCEDURE simple_cars_stats()
LANGUAGE SQL
AS $$
    SELECT 
        c.num AS car_number,
        c.mark,
        COALESCE(AVG(
            CASE 
                WHEN c.is_foreign THEN s.cost_foreign 
                ELSE s.cost_our 
            END
        ), 0) AS avg_cost,
        COUNT(w.id) AS works_count
    FROM cars c
    LEFT JOIN works w ON c.id = w.car_id
    LEFT JOIN services s ON w.service_id = s.id
    GROUP BY c.id, c.num, c.mark
    ORDER BY avg_cost DESC;
$$;
  
CREATE OR REPLACE PROCEDURE get_service_amount_by_masters(masterid1 INTEGER, masterid2 INTEGER, INOUT m1amount INTEGER DEFAULT 0, INOUT m2amount INTEGER DEFAULT 0) LANGUAGE plpgsql AS $$
  BEGIN
     SELECT COUNT(*) INTO m1amount FROM works WHERE master_id = masterid1 ;
     SELECT COUNT(*) INTO m2amount FROM works WHERE master_id = masterid2 ;
  END;
  $$;
