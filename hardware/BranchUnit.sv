`include "gDefine.svh"
import gDefine::*;

module BranchUnit #(
    parameter STACK_DEPTH = 16
)(
    clk, rstn,

    // Output execution mask
    maskOut,

    // instrValid
    instrValid,
    op,
    rid,
    ra,
    unfreezeCtrl,
    exitSig

);

    input clk, rstn;
    output Mask_t [`RSV_CAPACITY - 1 : 0] maskOut;
    input instrValid;
    input FuncCode_t op;
    input RsvID_t rid;
    input Vector_t ra;
    output logic [`RSV_CAPACITY - 1 : 0] unfreezeCtrl;
    output logic [`RSV_CAPACITY - 1 : 0] exitSig;

    Mask_t decode;

    localparam STACK_BLEN = $clog2(STACK_DEPTH);

    Mask_t [`RSV_CAPACITY - 1 : 0] [STACK_DEPTH - 1 : 0] pStack, eStack;
    logic [`RSV_CAPACITY - 1 : 0] [STACK_BLEN - 1 : 0] SP;
    Mask_t [`RSV_CAPACITY - 1 : 0] ExitFlagVec;

    always_comb begin
        for(int i = 0; i < `RSV_CAPACITY; i++) begin
            assign exitSig[i] = ExitFlagVec[i] == {16{1'b1}};
        end
    end

    // Mask Stack output logic

    always_comb begin
        for (int i = 0; i < `RSV_CAPACITY; i++) begin
            maskOut[i] = (~ExitFlagVec[i]) & (SP[i] == 0 ? {16{1'b1}} : pStack[i][SP[i] - 1]);
        end
    end

    always_comb begin
        for (int i = 0; i < `VEC_WIDTH; i++) begin
            // 0 means false
            decode[i] = ra[i] != 32'b0;
        end
    end

    always_ff @(posedge clk) begin
        if(rstn == 0) begin
            unfreezeCtrl[rid] <= 0;
            for (int i = 0; i < `RSV_CAPACITY; i++) begin
                SP[i] <= 0;
                ExitFlagVec[i] <= 0;
            end
        end
        else if(instrValid) begin
            unique case (op[4:0])
                CTRL_IF: begin
                    pStack[rid][SP[rid]] <= maskOut[rid] & decode;
                    eStack[rid][SP[rid]] <= maskOut[rid] & (~decode);
                    SP[rid] <= SP[rid] + 1;
                end
                CTRL_ELIF: begin
                    pStack[rid][SP[rid] - 1] <= eStack[rid][SP[rid] - 1] & decode;
                    eStack[rid][SP[rid] - 1] <= eStack[rid][SP[rid] - 1] & (~decode);
                end
                CTRL_ELSE: begin
                    pStack[rid][SP[rid] - 1] <= eStack[rid][SP[rid] - 1];
                end
                CTRL_ENDIF: begin
                    SP[rid] <= SP[rid] - 1;
                end
                EXIT: begin
                    ExitFlagVec[rid] <= ExitFlagVec[rid] | maskOut[rid];
                end
                default: begin

                end
            endcase
            unfreezeCtrl[rid] <= 1;
        end

        for (int i = 0; i < `RSV_CAPACITY; i++) begin
            if(unfreezeCtrl[i] == 1) 
                unfreezeCtrl[i] <= 0;
        end

        for (int i = 0; i < `RSV_CAPACITY; i++) begin
            if(exitSig[i] == 1) begin
                SP[i] <= 0;
                ExitFlagVec[i] <= 0;
            end
        end
    end
    
endmodule