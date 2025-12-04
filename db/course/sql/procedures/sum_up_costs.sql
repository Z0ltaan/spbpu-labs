CREATE
OR REPLACE PROCEDURE sum_up_costs (search_interval INTERVAL, INOUT cost INTEGER DEFAULT 0) LANGUAGE plpgsql AS $$ 
  BEGIN

SELECT
  COALESCE(
    sum(
      CASE
        WHEN c.is_foreign THEN s.cost_foreign
        ELSE s.cost_our
      END
    ),
    0
  ) into cost
FROM
  cars c
  LEFT JOIN works w ON (c.id = w.car_id)
  LEFT JOIN services s ON (w.service_id = s.id AND w.date_work >= CURRENT_DATE - search_interval);

  END;
  $$;

