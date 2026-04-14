delete from cars
where
  num = 'fffff';

BEGIN;

WITH
  selected_master AS (
    SELECT
      1 AS val
  )
DELETE FROM services
WHERE
  id IN (
    SELECT
      service_id
    FROM
      works w
    WHERE
      w.service_id IN (
        SELECT
          service_id
        from
          works
        where
          master_id = (
            SELECT
              val
            FROM
              selected_master
          )
      )
      AND w.service_id NOT IN (
        SELECT
          service_id
        FROM
          works
        where
          master_id != (
            SELECT
              val
            FROM
              selected_master
          )
      )
  );

COMMIT;

BEGIN;

WITH
  selected_master AS (
    SELECT
      1 AS val
  )
DELETE FROM services
WHERE
  id IN (
    SELECT
      service_id
    FROM
      works w
    WHERE
      w.service_id IN (
        SELECT
          service_id
        from
          works
        where
          master_id = (
            SELECT
              val
            FROM
              selected_master
          )
      )
      AND w.service_id NOT IN (
        SELECT
          service_id
        FROM
          works
        where
          master_id != (
            SELECT
              val
            FROM
              selected_master
          )
      )
  );

DELETE FROM masters
WHERE
  id = 1;

ROLLBACK;
