select
  count(*)
from
  services;

select
  sum(cost_our),
  sum(cost_foreign)
from
  services;

select
  *
from
  works
where
  date_work >= CURRENT_DATE - INTERVAL '1 month';

SELECT
  c.id,
  c.num,
  c.is_foreign,
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
  cars c
  LEFT JOIN works w ON (
    c.id = w.car_id
    AND w.date_work >= CURRENT_DATE - INTERVAL '1 year'
  )
  LEFT JOIN services s ON (w.service_id = s.id)
GROUP BY
  c.id
ORDER BY
  cost DESC;

SELECT
  c.is_foreign,
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
  cars c
  LEFT JOIN works w ON (c.id = w.car_id)
  LEFT JOIN services s ON (w.service_id = s.id)
GROUP BY
  c.is_foreign
ORDER BY
  cost DESC;
