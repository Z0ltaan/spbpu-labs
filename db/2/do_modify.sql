WITH
  factor as (
    SELECT
      1.15 AS val
  )
UPDATE services
SET
  cost_our = cost_our * (
    SELECT
      val
    FROM
      factor
  ),
  cost_foreign = cost_foreign * (
    SELECT
      val
    FROM
      factor
  );

BEGIN;

WITH
  factor as (
    SELECT
      10.00 AS val
  )
UPDATE services
SET
  cost_our = cost_our + (
    SELECT
      val
    FROM
      factor
  ),
  cost_foreign = cost_foreign + (
    SELECT
      val
    FROM
      factor
  )
WHERE
  id = (
    SELECT
      service_id
    FROM
      works w
    ORDER BY
      w.date_work DESC
    LIMIT
      1
  );

COMMIT;

BEGIN;

WITH
  factor as (
    SELECT
      10.00 AS val
  )
UPDATE services
SET
  cost_our = cost_our + (
    SELECT
      val
    FROM
      factor
  ),
  cost_foreign = cost_foreign + (
    SELECT
      val
    FROM
      factor
  )
WHERE
  id = (
    SELECT
      service_id
    FROM
      works w
    ORDER BY
      w.date_work DESC
    LIMIT
      1
  );

ROLLBACK;
