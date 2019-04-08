`include "ISA.svh"
`include "gDefine.svh"
import gDefine::*;

module IntExecPipeline #(
)(
    // Clk, rstn
    clk, rstn,
    // Input instr op
    op,
    // opValidIn, opValidOut,

    // IDs
    blockID,
    blockDim,
    warpID,
    rid,

    // Input operands
    a, b, imm,
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
    input clk;
    input rstn;

    input FuncCode_t op;
    // input opValidIn;
    // output logic opValidOut;

    input BlockID_t [`RSV_CAPACITY - 1 : 0] blockID;
    input BlockDim_t [`RSV_CAPACITY - 1 : 0] blockDim;
    input WarpID_t [`RSV_CAPACITY - 1 : 0] warpID;
    input RsvID_t rid;

    input Vector_t a;
    input Vector_t b;
    input Word_t imm;
    output Vector_t out;

    // input Mask_t maskIn;
    // input [`REG_BITS - 1 : 0] dstRegIn;
    // input dstRegValidIn, dstRegTypeIn;
    // output logic dstRegValidOut, dstRegTypeOut;

    // output logic [`VEC_WIDTH - 1 : 0] maskOut;
    // output logic [`REG_BITS - 1 : 0] dstRegOut;

    input stall;

    Vector_t resultVec;
    // assign out = resultVec;

    genvar lane;
    generate;
        for(lane = 0; lane < `VEC_WIDTH; lane++) begin
            Scalar_t op1 = a[lane];
            Scalar_t op2 = b[lane];

            Scalar_t result;

            logic [4 : 0] lead_zero;
            logic [4 : 0] trail_zero;

            count_lead_zero clz(op1, lead_zero);
            count_trail_zero ctz(op1, trail_zero);

            Scalar_t diff;
            logic borrow;

            assign {borrow, diff} = {1'b0, op1} - {1'b0, op2};

            logic negative;
            assign negative = diff[`DWIDTH - 1];
            
            logic zero;
            assign zero = diff == 0;

            Scalar_t rShiftArith, rShiftLogic;
            assign rShiftArith = op1 >>> imm[4 : 0];
            assign rShiftLogic = op1 >> imm[4 : 0];

            Scalar_t lShiftLogic;
            assign lShiftLogic = op1 << imm[4 : 0];

            always_comb begin
                if (op[7 : 5] == 3'b000) begin
                    unique case(op[4 : 0])
                        // R type instructions
                        OP_OR:      result = op1 | op2;
                        OP_AND:     result = op1 & op2;
                        OP_XOR:     result = op1 ^ op2;

                        OP_ADD:     result = op1 + op2;

                        OP_SUB:     result = op1 - op2;
                        
                        OP_MULT:    result = op1 * op2;
                        
                        // OP_MOVE:    result = op1;
                        OP_CMPEQ_I: result = {{31{1'b0}}, zero};
                        OP_CMPNE_I: result = {{31{1'b0}}, !zero};
                        OP_CMPGT_I: result = {{31{1'b0}}, signed'(op1) > signed'(op2)};
                        OP_CMPGE_I: result = {{31{1'b0}}, signed'(op1) >= signed'(op2)};
                        OP_CMPLT_I: result = {{31{1'b0}}, signed'(op1) < signed'(op2)};
                        OP_CMPLE_I: result = {{31{1'b0}}, signed'(op1) <= signed'(op2)};
                        OP_CMPGT_U: result = {{31{1'b0}}, op1 > op2};
                        OP_CMPGE_U: result = {{31{1'b0}}, op1 >= op2};
                        OP_CMPLT_U: result = {{31{1'b0}}, op1 < op2};
                        OP_CMPLE_U: result = {{31{1'b0}}, op1 <= op2};
                        default:    result = 0;
                    endcase
                end
                else if (op[7 : 5] == 3'b010) begin
                    // Immediate type instructions
                    unique case(op[4 : 0])
                        OP_ASHR:    result = rShiftArith;
                        OP_SHR:     result = rShiftLogic;
                        OP_SHL:     result = lShiftLogic;
                        OP_CLZ:     result = {{27{1'b0}}, lead_zero};
                        OP_CTZ:     result = {{27{1'b0}}, trail_zero};

                        OP_ADDI,
                        OP_ADDIU:   result = op1 + imm;

                        OP_SUBI,
                        OP_SUBIU:   result = op1 - imm;
                        
                        OP_MULTI:   result = op1 * imm;

                        /* verilator lint_off WIDTH */
                        SYS_BLOCK_DIM:  result = blockDim[rid];
                        SYS_BLOCK_IDX:  result = blockID[rid];
                        SYS_THREAD_IDX: result = (warpID[rid] << 4) + lane;
                        /* verilator lint_on WIDTH */

                        default:    result = 0;
                    endcase

                end
                else if (op[7 : 5] == 3'b101) begin
                    // Immediate type instructions
                    unique case(op[2 : 0])
                        OP_LLI_I:   result = imm;
                        OP_LUI_I:   result = imm << 12;
                        default:    result = 0;
                    endcase
                end
            end

            assign resultVec[lane] = result;

        end
    endgenerate

    always_ff @(posedge clk) begin
        if(!stall) begin
            out <= resultVec;
        end
        else begin
            out <= out;
        end
    end

endmodule