`include "gDefine.svh"

typedef struct packed {
    logic busy;
} MapTableEntry_t;

module Scoreboard #(
    parameter ScoreBoardID = 0
)(
    // Input
    clk, rstn,

    // Master control
    instrValid,
    instrTag,
    instrType,
    instrFuncCode,
    
    // Dst reg logic input
    rdIdx,
    rdType,
    hasRd,

    r0Idx, r1Idx, r2Idx, 
    r0Type, r1Type, r2Type,
    regCountI, regCountF,
    hasImm,
    imm,

    // from write back
    wbValidI,
    wbThreadI,
    wbVregI,

    wbValidF,
    wbThreadF,
    wbVregF,

    // Operand output
    reqR0Idx, reqR1Idx, reqR2Idx,
    reqR0Type, reqR1Type, reqR2Type,

    // Dst reg output
    rdTypeOut,
    hasRdOut,

    // Ready to dispatch
    Ready,
    funcUnit,
    funcCode,
    vrdIdxOut,
    
    regCountOutI, regCountOutF,
    hasImmOut,
    immOut,

    // Dispatch Ack input
    dispatchAck,

    // Tell context manager to load new instr
    dispatched
);
    input clk, rstn;

    // From decode unit
    input instrValid;
    input [$clog2(`SCOREBOARD_SIZE) - 1 : 0] instrTag;
    input FuncUnitType_t instrType;
    input FuncCode_t instrFuncCode;

    input VRegIdx_t r0Idx, r1Idx, r2Idx, rdIdx;
    input r0Type, r1Type, r2Type, rdType;
    input hasRd;

    input [1 : 0] regCountI;
    input [1 : 0] regCountF;
    input hasImm;
    input [`DWIDTH - 1 : 0] imm;
    // From decode unit end

    // From WB
    input logic wbValidI;
    input RsvID_t wbThreadI;
    input VRegIdx_t wbVregI;

    input logic wbValidF;
    input RsvID_t wbThreadF;
    input VRegIdx_t wbVregF;

    output VRegIdx_t reqR0Idx, reqR1Idx, reqR2Idx;
    output logic reqR0Type, reqR1Type, reqR2Type;

    // To execution unit
    output logic hasRdOut;
    output logic rdTypeOut;

    output logic [1 : 0] regCountOutI;
    output logic [1 : 0] regCountOutF;
    output logic hasImmOut;
    output logic [`DWIDTH - 1 : 0] immOut;

    // Dispatch port signals
    output logic Ready;
    output FuncUnitType_t funcUnit;
    output FuncCode_t funcCode;
    output VRegIdx_t vrdIdxOut;

    input dispatchAck;
    // Ctrl
    output logic dispatched;

    MapTableEntry_t [`MAX_REG_THREAD - 1 : 0] mapTableI;
    MapTableEntry_t [`MAX_REG_THREAD - 1 : 0] mapTableF;

    logic raPending, rbPending, rcPending;

    logic dstReady;

    assign Ready = {raPending, rbPending, rcPending} == 3'b000 && dstReady && !dispatched;

    always_ff @(posedge clk) begin
        if(rstn == 0) begin
            dispatched <= 1;
            // {m_reqR0Valid, m_reqR1Valid, m_reqR2Valid} <= 3'b0;
            // {needCDBr0, needCDBr1, needCDBr2} <= 3'b0;
            for (int i = 0; i < `MAX_REG_THREAD; i++) begin
                mapTableI[i].busy <= 0;
                mapTableF[i].busy <= 0;
            end
        end
        else begin
            if(instrValid && instrTag == ScoreBoardID) begin
                funcUnit <= instrType;
                funcCode <= instrFuncCode;
                dispatched <= 0;

                {reqR0Type, reqR1Type, reqR2Type, rdTypeOut} <= {r0Type, r1Type, r2Type, rdType};
                {raPending, rbPending, rcPending} <= 3'b000;

                hasRdOut <= hasRd;
                rdTypeOut <= rdType;

                hasImmOut <= hasImm;

                regCountOutI <= regCountI;
                regCountOutF <= regCountF;

                if (hasRd) begin
                    if (rdType == 0)
                        mapTableI[rdIdx].busy <= 1;
                    else 
                        mapTableF[rdIdx].busy <= 1;
                    vrdIdxOut <= rdIdx;
                end
                reqR0Idx <= r0Idx; // + ScoreBoardID * `MAX_REG_THREAD;
                reqR1Idx <= r1Idx; // + ScoreBoardID * `MAX_REG_THREAD;
                reqR2Idx <= r2Idx; // + ScoreBoardID * `MAX_REG_THREAD;

                if(regCountI + regCountF > 0) begin
                    if (r0Type == 0)
                        raPending <= mapTableI[r0Idx].busy == 1;
                    else 
                        raPending <= mapTableF[r0Idx].busy == 1;
                end

                if(regCountI + regCountF > 1) begin
                    if (r1Type == 0)
                        rbPending <= mapTableI[r1Idx].busy == 1;
                    else
                        rbPending <= mapTableF[r1Idx].busy == 1;
                end
                if(regCountI + regCountF > 2) begin
                    if (r2Type == 0)
                        rcPending <= mapTableI[r2Idx].busy == 1;
                    else
                        rcPending <= mapTableF[r2Idx].busy == 1;
                end

                if(rdTypeOut == 0)
                    dstReady <= mapTableI[vrdIdxOut].busy == 0;
                else 
                    dstReady <= mapTableF[vrdIdxOut].busy == 0;
                
                immOut <= imm;

                // unique case (regCountI + regCountF)
                //     'd0: begin
                //         {raPending, rbPending, rcPending} <= 3'b000;
                //         if (hasImm) begin
                //             immOut <= imm;
                //         end
                //     end
                //     'd1: begin
                //         {raPending, rbPending, rcPending} <= 3'b100;
                //         if (hasImm) begin
                //             immOut <= imm;
                //         end
                //     end
                //     'd2: begin
                //         {raPending, rbPending, rcPending} <= 3'b110;
                //         if (hasImm) begin
                //             immOut <= imm;
                //         end
                //     end
                //     'd3: begin
                //         {raPending, rbPending, rcPending} <= 3'b111;
                //     end
                //     default: 
                //         {raPending, rbPending, rcPending} <= 3'b000;
                    
                // endcase
            end
            else if (dispatchAck == 1) begin
                dispatched <= 1;
            end
            if (wbThreadI == ScoreBoardID && wbValidI) begin
                if (wbVregI == reqR0Idx && reqR0Type == 0) begin
                    raPending <= 0;
                    mapTableI[reqR0Idx] <= 0;
                end
                if (wbVregI == reqR1Idx && reqR1Type == 0) begin
                    rbPending <= 0;
                    mapTableI[reqR1Idx] <= 0;
                end
                if (wbVregI == reqR2Idx && reqR2Type == 0) begin
                    rcPending <= 0;
                    mapTableI[reqR2Idx] <= 0;
                end
                if (wbVregI == vrdIdxOut && rdTypeOut == 0) begin
                    dstReady <= 1;
                    mapTableI[vrdIdxOut] <= 0;
                end
            end
            if (wbThreadF == ScoreBoardID && wbValidF) begin
                if (wbVregF == reqR0Idx && reqR0Type == 1) begin
                    raPending <= 0;
                    mapTableF[reqR0Idx] <= 0;
                end
                if (wbVregF == reqR1Idx && reqR1Type == 1) begin
                    rbPending <= 0;
                    mapTableF[reqR1Idx] <= 0;
                end
                if (wbVregF == reqR2Idx && reqR2Type == 1) begin
                    rcPending <= 0;
                    mapTableF[reqR2Idx] <= 0;
                end
                if (wbVregF == vrdIdxOut && rdTypeOut == 0) begin
                    dstReady <= 1;
                    mapTableF[vrdIdxOut] <= 0;
                end
            end
        end
    end






endmodule