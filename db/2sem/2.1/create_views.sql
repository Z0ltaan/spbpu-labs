CREATE VIEW foreign_cars AS
SELECT
  num,
  color,
  mark
FROM
  cars
WHERE
  is_foreign = TRUE;

  -- multitable
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
