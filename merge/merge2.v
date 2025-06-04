module merge2(
    input clk,
    input reset,
    input [79:0] input_array,
    output reg [79:0] output_array,
    output reg processing
);

// Merge two subarrays
function [79:0] mergeArrays;
    input [79:0] num;
    input integer low, mid, hi;
    reg [79:0] temp;
    integer i, j, k;
    reg [7:0] left_val, right_val;
begin
    temp = num;
    i = low;
    j = mid + 1;
    k = low;

    while (i <= mid && j <= hi) begin
        left_val = temp[i*8 +: 8];
        right_val = temp[j*8 +: 8];
        if (left_val <= right_val) begin
            temp[k*8 +: 8] = left_val;
            i = i + 1;
        end else begin
            temp[k*8 +: 8] = right_val;
            j = j + 1;
        end
        k = k + 1;
    end

    // Copy remaining elements from left half
    while (i <= mid) begin
        temp[k*8 +: 8] = temp[i*8 +: 8];
        i = i + 1;
        k = k + 1;
    end

    // Copy remaining elements from right half
    while (j <= hi) begin
        temp[k*8 +: 8] = temp[j*8 +: 8];
        j = j + 1;
        k = k + 1;
    end

    mergeArrays = temp;
end
endfunction

// Iterative merge sort
function [79:0] sort;
    input [79:0] num;
    integer sz, low, mid, hi;
    reg [79:0] temp;
begin
    temp = num;
    for (sz = 1; sz < 10; sz = sz * 2) begin
        for (low = 0; low < 10 - sz; low = low + 2 * sz) begin
            mid = low + sz - 1;
            hi = (low + 2 * sz - 1 < 9) ? (low + 2 * sz - 1) : 9;
            temp = mergeArrays(temp, low, mid, hi);
        end
    end
    sort = temp;
end
endfunction

always @(posedge clk or posedge reset) begin
    if (reset) begin
        output_array <= 0;
        processing <= 0;
    end else begin
        output_array <= sort(input_array);
        processing <= 1;
    end
end

endmodule
