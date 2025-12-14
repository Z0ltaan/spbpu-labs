module merge(
    input clk,
    input [79:0] input_array,
    output reg [79:0] output_array
);

function [79:0] mergeArrays;
input [79:0] num;
input integer low, mid, hi;
reg [7:0] sorted[0:9];
reg [7:0] working[0:9];
integer i, j, k;

begin
	working[0] = num[7:0];
	working[1] = num[15:8];
	working[2] = num[23:16];
	working[3] = num[31:24];
	working[4] = num[39:32];
	working[5] = num[47:40];
	working[6] = num[55:48];
	working[7] = num[63:56];
	working[8] = num[71:64];
	working[9] = num[79:72];

	i = low;
	j = mid + 1;
	k = 0;

	for (k = 0; k < 10; k = k + 1) begin
    if (i == mid + 1) begin
      sorted[k] = working[j];
			j = j + 1;
    end else if (j == hi + 1) begin
      sorted[k] = working[i];
			i = i + 1;
    end else if (working[i] < working[j]) begin
      sorted[k] = working[i];
			i = i + 1;
    end else begin
      sorted[k] = working[j];
			j = j + 1;
		end
	end

	for (i = low; i <= hi; i = i + 1) begin
    	working[i] = sorted[i - low];
	end

	mergeArrays = {
									working[0], working[1], working[2],
									working[3], working[4], working[5],
									working[6], working[7], working[8],
									working[9]
								};
end
endfunction

function integer min;
input integer left, right;
integer temp;
begin
	temp = 0;
	if (left < right) begin
		temp = left;
	end else begin
		temp = right;
	end
	min =	temp;
end
endfunction

function [79:0] sort;
input [79:0] num;
integer sz, low;
reg [7:0] working[0:9];
reg [79:0] temp;
reg [7:0] sorted[0:9];
begin
	sz = 1;
	low = 0;

	working[0] = num[7:0];
	working[1] = num[15:8];
	working[2] = num[23:16];
	working[3] = num[31:24];
	working[4] = num[39:32];
	working[5] = num[47:40];
	working[6] = num[55:48];
	working[7] = num[63:56];
	working[8] = num[71:64];
	working[9] = num[79:72];

	for (sz = 1; sz < 10; sz = sz * 2) begin
		for (low = 0; low < 10 - sz; low = low + 2 * sz) begin
			temp = mergeArrays({
												working[0], working[1], working[2],
												working[3], working[4], working[5],
												working[6], working[7], working[8],
												working[9]
											}, low, low + sz - 1, min(low + 2 * sz - 1, 9));

			working[0] = temp[7:0];
			working[1] = temp[15:8];
			working[2] = temp[23:16];
			working[3] = temp[31:24];
			working[4] = temp[39:32];
			working[5] = temp[47:40];
			working[6] = temp[55:48];
			working[7] = temp[63:56];
			working[8] = temp[71:64];
			working[9] = temp[79:72];
		end
	end

	sort = {
						working[0], working[1], working[2],
						working[3], working[4], working[5],
						working[6], working[7], working[8],
						working[9]
				 };

end
endfunction

always @(posedge(clk)) begin
	output_array <= sort(input_array);
end

endmodule
