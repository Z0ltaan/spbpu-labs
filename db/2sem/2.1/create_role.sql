CREATE ROLE db_operator;
GRANT UPDATE (color) ON foreign_cars TO db_operator;
GRANT SELECT ON foreign_cars TO db_operator;
GRANT INSERT ON services_cap TO db_operator;
grant INSERT on works to db_operator;
grant INSERT on works to db_operator;
grant SELECT on works to db_operator;
GRANT USAGE ON SCHEMA public TO db_operator;
GRANT db_operator TO test;

