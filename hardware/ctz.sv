// This module is inspired by 
// https://electronics.stackexchange.com/questions/196914/verilog-synthesize-high-speed-leading-zero-count

module count_trail_zero #(
    parameter W_IN = 32, // Must be power of 2, >=2
    parameter W_OUT = $clog2(W_IN) // Let this default
) (
    input wire  [W_IN - 1 : 0] in,
    output wire [W_OUT - 1 : 0] out
);

generate
    if (W_IN == 2) begin: base
        assign out = !in[0];
    end 
    else begin: recurse
        wire [W_OUT - 2 : 0] half_count;
        wire [W_IN / 2 - 1 : 0] lhs = in[W_IN / 2 +: W_IN / 2];
        wire [W_IN / 2 - 1 : 0] rhs = in[0        +: W_IN / 2];
        wire right_empty = ~|rhs;

        count_trail_zero #(
            .W_IN (W_IN / 2)
        ) inner (
            .in  (right_empty ? lhs : rhs),
            .out (half_count)
        );

        assign out = {right_empty, half_count};
    end
endgenerate

endmodule