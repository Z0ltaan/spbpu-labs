GRANT SELECT, INSERT, UPDATE ON TABLE cars TO test;

GRANT SELECT (name, cost_our, cost_foreign), 
      UPDATE (cost_our, cost_foreign) 
ON TABLE services TO test;

GRANT SELECT ON TABLE users TO test;
GRANT SELECT ON TABLE foreign_cars TO test;
