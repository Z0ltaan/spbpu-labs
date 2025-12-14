CREATE TYPE score_table_row as (id INTEGER, name TEXT, cars INTEGER);

CREATE
OR REPLACE PROCEDURE get_scores_table (cmonth INTEGER, cyear INTEGER, INOUT scores score_table_row[] DEFAULT ARRAY[]::score_table_row[]) LANGUAGE plpgsql AS $$
DECLARE 
  pos INTEGER := 0;
  -- cyear INTEGER := EXTRACT(year from CURRENT_DATE);
  crow score_table_row;

    master_cursor CURSOR FOR
SELECT
    w.master_id,
    m.name AS master_name,
    COUNT(DISTINCT w.car_id) AS different_cars_serviced
FROM works w
JOIN masters m ON m.id = w.master_id
WHERE EXTRACT(MONTH FROM w.date_work) = cmonth
    AND EXTRACT(YEAR FROM w.date_work) = cyear
GROUP BY w.master_id, m.name
ORDER BY different_cars_serviced DESC, master_name
LIMIT 5;

  BEGIN
    OPEN master_cursor;

    LOOP
        FETCH master_cursor INTO crow;
        EXIT WHEN NOT FOUND;

        scores := scores || crow;

    END LOOP;

    CLOSE master_cursor;
  END;
  $$;

