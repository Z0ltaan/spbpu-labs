#ifndef CONTROLLER_TEST_H
#define CONTROLLER_TEST_H

#include <systemc.h>

SC_MODULE(i2c_controller_test)
{
  sc_in<bool>        clk;
  sc_out<bool>       reset;
  sc_out<sc_uint<8>> data;
  sc_out<bool>       valid;
  sc_out<sc_logic>   scl;
  sc_out<sc_logic>   sda;
  sc_in<bool>        ready;
  sc_in<bool>        scl_o;
  sc_in<bool>        scl_m;
  sc_in<bool>        sda_o;
  sc_in<bool>        sda_m;

  void generate_valid();
  void set_scl_sda();

  SC_CTOR(i2c_controller_test)
  {
    SC_THREAD(generate_valid);
    sensitive << clk.pos();

    SC_METHOD(set_scl_sda);
    sensitive << scl_m << sda_m << scl_o << sda_o;
  }
};

#endif