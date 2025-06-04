#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <systemc.h>

SC_MODULE(i2c_controller) {
  sc_in<bool>       clk;
  sc_in<bool>       reset;
  sc_in<sc_uint<8>> data;
  sc_in<bool>       valid;
  sc_in<sc_logic>   scl;
  sc_in<sc_logic>   sda;
  sc_out<bool>      ready;
  sc_out<bool>      scl_o;
  sc_out<bool>      scl_m;
  sc_out<bool>      sda_o;
  sc_out<bool>      sda_m;

  enum state_t { IDLE, SEND_DATA, WAIT_ACK, DONE };

  state_t state;
  sc_signal<sc_uint<8>> data_reg;
  // sc_clock scl_when_sending;

  unsigned short int count;
  void fsm();
  void sendAddress();
  void setSclM();
  void setSdaO();
  void setSdaM();

  SC_CTOR(i2c_controller):
  state(IDLE),
  count(0)
  // scl_when_sending("cl1k", 10, SC_NS)
  {
    SC_METHOD(fsm);
    sensitive << clk.pos() << reset;

    SC_METHOD(sendAddress);
    sensitive << clk << reset;

    SC_METHOD(setSclM);
    sensitive << scl_o << sda_m.neg() << reset;

    SC_METHOD(setSdaO);
    sensitive << scl_o << reset << sda_m.pos();

    SC_METHOD(setSdaM);
    sensitive << clk << reset;
  }
};

#endif