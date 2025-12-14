`timescale 1ns / 1ps
module merge_tb;
  // Clock anh reset signals
  reg clk = 0;
  reg [79:0] sorted = {8'h45, 8'h12,8'hF4,8'h43,8'h25,8'hFF,8'h11,8'h35,8'hD6,8'hA5};
  wire [79:0] out;

  initial begin
  	$dumpfile("merge.vcd");
  	$dumpvars(0, merge_tb);

    #40 clk = 1'b1;
    #10 clk = 1'b0;
    #500 $finish;
  end

  merge dut (clk, sorted, out);

endmodule
