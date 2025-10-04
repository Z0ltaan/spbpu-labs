CREATE TABLE cars (
    id INTEGER PRIMARY KEY ,
    num VARCHAR(10) NOT NULL UNIQUE,
    color VARCHAR(30),
    mark VARCHAR(80),
    is_foreign BOOLEAN NOT NULL
);
CREATE TABLE masters (
  id INTEGER PRIMARY KEY ,
  name VARCHAR(200) NOT NULL
);
CREATE TABLE services (
    id INTEGER PRIMARY KEY ,
    name VARCHAR(200) NOT NULL,
    cost_our NUMERIC(10,2),
    cost_foreign NUMERIC(10,2)
);
CREATE TABLE works (
  id INTEGER PRIMARY KEY ,
    date_work DATE NOT NULL,
    master_id INTEGER NOT NULL,
    car_id INTEGER NOT NULL,
    service_id INTEGER NOT NULL
);

