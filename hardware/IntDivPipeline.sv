`include "ISA.svh"
`include "gDefine.svh"

module IntMultPipeline #(
)(
    // Clk, rst
    clk, rst,
    // Input instr op
    op,

    // Input operands
    a, b, imm,
    // Output
    out,

    // Stall 
    stall,
    busy
);
    input clk, rst;

    input FuncCode_t op;

    input Vector_t a;
    input Vector_t b;
    input Word_t imm;
    output Vector_t out;

    input stall;
    output logic busy;

    Vector_t resultVec;
    assign out = resultVec;

    genvar lane;
    generate;
        for(lane = 0; lane < `VEC_WIDTH; lane++) begin
            Scalar_t op1 = a[lane];
            Scalar_t op2 = b[lane];

            Scalar_t result;


            // always_comb begin
            //     if (op[7 : 5] == 3'b000) begin
            //         // R type instructions
            //         unique case(op[4  :0])
            //             OP_MULT:    

            //             default:
            //         endcase
            //     end
            //     else if (op[7 : 5] == 3'b010) begin
            //         // Immediate type instructions


            //     end
            //     else begin
            //         result <= 0;
            //     end
            // end

            assign resultVec[lane] = result;

        end
    endgenerate

endmodule