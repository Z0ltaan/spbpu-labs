BEGIN;
  UPDATE accounts SET balance = 900 WHERE id = 1;
COMMIT;
