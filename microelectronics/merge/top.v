module top(
    input wire clk,
    input wire reset,
    output wire [79:0] sorted_data
);

    // Входные данные (можно заменить на свои)
    wire [7:0] working [0:9];
    assign working[0] = 8'h45;
    assign working[1] = 8'h12;
    assign working[2] = 8'h78;
    assign working[3] = 8'h23;
    assign working[4] = 8'h56;
    assign working[5] = 8'h89;
    assign working[6] = 8'h34;
    assign working[7] = 8'h67;
    assign working[8] = 8'h90;
    assign working[9] = 8'h01;

		merge m (
			.clk(clk),
			.reset(reset),
			.input_array({
												working[0], working[1], working[2],
												working[3], working[4], working[5],
												working[6], working[7], working[8],
												working[9]
											}),
			.output_array(sorted_data)
		);


endmodule
