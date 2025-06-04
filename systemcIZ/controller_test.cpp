#include "controller_test.h"

void i2c_controller_test::generate_valid()
{
  reset.write(1);
  wait(20, SC_NS);
  reset.write(0);
  data.write(0x42);
  wait(10, SC_NS);
  valid.write(1);
  wait(20, SC_NS);
  data.write(0x0);
  valid.write(0);
  wait(1000, SC_NS);
}

void i2c_controller_test::set_scl_sda()
{
  scl.write(scl_m ? sc_logic(scl_o) : sc_logic('z'));
  sda.write(sda_m ? sc_logic(sda_o) : sc_logic('z'));
}