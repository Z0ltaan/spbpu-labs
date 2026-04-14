CREATE VIEW services_cap AS
SELECT
  s.id,
  s.name,
  w.car_id,
  sum(
    CASE
      WHEN c.is_foreign THEN s.cost_foreign
      ELSE s.cost_our
    END
  ) AS cost
FROM
  services s
  JOIN works w ON s.id = w.service_id
  JOIN cars c ON w.car_id = c.id
GROUP BY
  s.id,
  s.name,
  w.car_id
HAVING
  sum(
    CASE
      WHEN c.is_foreign THEN s.cost_foreign
      ELSE s.cost_our
    END
  ) > 10000;

-- / /
CREATE VIEW masters_acc_year AS
SELECT
  m.id,
  COALESCE(
    sum(
      CASE
        WHEN c.is_foreign THEN s.cost_foreign
        ELSE s.cost_our
      END
    ),
    0
  ) AS cost
FROM
  masters m
  LEFT JOIN works w ON m.id = w.master_id
  AND w.date_work > CURRENT_DATE - INTERVAL '1 year'
  LEFT JOIN services s ON s.id = w.service_id
  LEFT JOIN cars c ON w.car_id = c.id
GROUP BY
  m.id;
