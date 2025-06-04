`timescale 1ns / 1ps
module top_tb;
  // Clock and reset signals
  reg clk;
  reg reset;
  reg [79:0] sorted;

  top dut (
    .clk (clk),
    .reset (reset),
    .sorted_data (sorted)
  );

  // generate the clock
  initial begin
    clk = 1'b0;
    #40 clk = 1'b1;
    #10 clk = 1'b0;
  end
endmodule
