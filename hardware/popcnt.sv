module popcnt #(
    parameter WIDTH = 32,
    parameter CWIDTH = $clog2(WIDTH) + 1
)(
    // Comb logic only
    input [WIDTH - 1 : 0] in,
    output logic [CWIDTH - 1 : 0] pcnt
);
    always_comb begin
        pcnt = 0;
        for(int i = 0; i < WIDTH; i ++) begin
            /* verilator lint_off WIDTH */
            pcnt = pcnt + in[i];
            /* verilator lint_on WIDTH */
        end
    end

endmodule