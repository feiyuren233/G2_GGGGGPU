`include "ISA.svh"
`include "gDefine.svh"

import gDefine::*;

module FPExecPipeline #(
    // WARNING! HAS TO BE EXACT SAME AS FLOATING POINT IP DELAY 
)(
    // Clk, rst
    clk, rst,
    // Input instr op
    op,
    // opValidIn, opValidOut,

    // Input operands
    a, b, c,
    // Output
    out,

    // Mask, output logical regs
    // maskIn, maskOut,
    // dstRegIn, dstRegOut,
    // dstRegValidIn, dstRegTypeIn,
    // dstRegValidOut, dstRegTypeOut,

    // Stall 
    stall
);
    input clk, rst;
    input FuncCode_t op;
    // input opValidIn;
    // output logic opValidOut;

    input Vector_t a, b, c;
    output Vector_t out;

    // input [`VEC_WIDTH - 1 : 0] maskIn;
    // input [`REG_BITS - 1 : 0] dstRegIn;
    // input dstRegValidIn, dstRegTypeIn;
    // output logic dstRegValidOut, dstRegTypeOut;

    // output logic [`VEC_WIDTH - 1 : 0] maskOut;
    // output logic [`REG_BITS - 1 : 0] dstRegOut;

    input stall;

    Vector_t resultVec;

    // Per SIMD signals
    logic FMA_mode;

    always_comb begin
        unique case(op[4 : 0])
            OP_ADD_F,
            OP_MUL_F,
            OP_FMA_ADD: FMA_mode = 1;

            OP_SUB_F,
            OP_FMA_SUB: FMA_mode = 0;

            default: FMA_mode = 1;
        endcase
    end

`ifdef VERILATOR
    // Connect to simulation module
    genvar lane;
    generate;
        for(lane = 0; lane < `VEC_WIDTH; lane++) begin
            //  a * b + / - c
            Scalar_t op0;
            Scalar_t op1;
            Scalar_t op2;

            always_comb begin
                op0 = a[lane];
                op1 = b[lane];
                op2 = c[lane];

                if(op[7 : 5] == 3'b000) begin
                    unique case(op[4 : 0])
                        OP_ADD_F,
                        OP_SUB_F:   op0 = 32'h3f800000; // Dec. 1.0

                        OP_MUL_F:   op2 = 32'h0;        // Dec. 0.0

                        default: op0 = 32'h3f800000;
                    endcase
                end
            end

            Scalar_t result;

            FMA_sim fma (clk, !stall, rst, op0, op1, op2, FMA_mode, result);

            assign resultVec[lane] = result;
        end
    endgenerate


`else
    // Else clause, connect to real floating point ip


`endif


endmodule