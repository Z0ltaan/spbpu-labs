#include "controller.h"
#include "controller_test.h"

int sc_main(int argc, char *argv[]) {
  // create modules
  i2c_controller      controller("controller");
  i2c_controller_test test("testo");

  sc_clock              s_clk("clk", 10, SC_NS);
  sc_signal<bool>       s_reset("reset");
  sc_signal<sc_uint<8>> s_data("data");
  sc_signal<bool>       s_valid("valid");
  sc_signal<sc_logic>   s_scl("scl");
  sc_signal<sc_logic>   s_sda("sda");
  sc_signal<bool>       s_ready("ready");
  sc_signal<bool>       s_scl_o("scl_o");
  sc_signal<bool>       s_scl_m("scl_m");
  sc_signal<bool>       s_sda_o("sda_o");
  sc_signal<bool>       s_sda_m("sda_m");


  controller.clk  (s_clk);
  controller.reset(s_reset);
  controller.data (s_data);
  controller.valid(s_valid);
  controller.scl  (s_scl);
  controller.sda  (s_sda);
  controller.ready(s_ready);
  controller.scl_o(s_scl_o);
  controller.scl_m(s_scl_m);
  controller.sda_o(s_sda_o);
  controller.sda_m(s_sda_m);

  test.clk  (s_clk);
  test.reset(s_reset);
  test.data (s_data);
  test.valid(s_valid);
  test.scl  (s_scl);
  test.sda  (s_sda);
  test.ready(s_ready);
  test.scl_o(s_scl_o);
  test.scl_m(s_scl_m);
  test.sda_o(s_sda_o);
  test.sda_m(s_sda_m);


  // create output trace file
  sc_trace_file *tf = sc_create_vcd_trace_file("sciz");
  tf->set_time_unit(1, SC_NS);

  // all changed in signals will be auto output'ed to trace file
  sc_trace(tf, s_clk,   "clk");
  sc_trace(tf, s_reset, "reset");
  sc_trace(tf, s_data,  "data");
  sc_trace(tf, s_valid, "valid");
  sc_trace(tf, s_scl,   "scl");
  sc_trace(tf, s_sda,   "sda");
  sc_trace(tf, s_ready, "ready");
  sc_trace(tf, s_scl_o, "scl_o");
  sc_trace(tf, s_scl_m, "scl_m");
  sc_trace(tf, s_sda_o, "sda_o");
  sc_trace(tf, s_sda_m, "sda_m");

  sc_start(9000, SC_NS);

  sc_close_vcd_trace_file(tf);
  return 0;
}
