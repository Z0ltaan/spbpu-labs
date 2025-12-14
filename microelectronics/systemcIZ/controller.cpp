#include "controller.h"

void i2c_controller::setSdaO() {
  if (reset) {
    sda_o.write(1);
  } else if (sda_m && !scl_m)
    sda_o.write(0);
  else if (state == SEND_DATA && !scl_o) {
    sda_o.write(data_reg.read()[count]);
  } else if (state == WAIT_ACK && !scl_o) {
    sda_o.write(0);
  } else if (ready)
    sda_o.write(1);
}

void i2c_controller::setSclM() {
  if (sda_m || state == WAIT_ACK || state == DONE)
    scl_m.write(1);
  else
    scl_m.write(0);
}

void i2c_controller::setSdaM()
{
  if (state == IDLE || reset)
  {
    sda_m.write(0);
  }
  else if (state == WAIT_ACK && !clk)
  {
    sda_m.write(0);
  }
  else if (state == DONE && !clk)
  {
    sda_m.write(1);
  }
  else if (state == SEND_DATA)
  {
    sda_m.write(1);
  }
}

void i2c_controller::sendAddress() {
  if (state == SEND_DATA || state == WAIT_ACK || state == DONE)
    scl_o.write(clk);
  else
    scl_o.write(1);
}

void i2c_controller::fsm() {
  if (reset.read()) {
    state = IDLE;
    // ready.write(1);
    count = 0;
  } else {
    if (state == IDLE) {
      ready.write(1);
      if (valid) {
        ready.write(false);
        data_reg.write(data.read());
        state = SEND_DATA;
        count = 7;
      }
    } else if (state == SEND_DATA) {
      if (count == 0) {
        state = WAIT_ACK;
      } else {
        count = count - 1;
      }
    } else if (state == WAIT_ACK) {
      state = DONE;
    } else if (state == DONE) {
      // ready.write(true);
      state = IDLE;
    }
  }
}
