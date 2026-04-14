begin transaction isolation level repeatable read;
  select * from cars;

  update cars set color=(select color from cars where id = 3) where id = 1;

  select * from cars;

  commit;
