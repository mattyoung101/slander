module top(
    input logic clk,
    input logic a,
    output logic out1,
    output logic out2
);

always_comb begin
    out1 = a;
    out2 = a;
end

always_ff @(posedge clk) begin
    out2 <= a;
end

always_ff @(posedge clk) begin
    out2 <= a;
end

endmodule
