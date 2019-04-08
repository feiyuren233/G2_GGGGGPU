`include "gDefine.svh"
`include "WBBroadcast.svh"

// enum int {
//     READY = 0,
//     WAIT = 1,
//     REQUEST = 2,
//     EMPTY = 3
// } resvState_e;

// // Information for a warp (16 lanes)

typedef struct packed {
    GRegIdx_t [`MAX_REG_THREAD - 1 : 0] Vreg2Greg;
    logic [`MAX_REG_THREAD - 1 : 0] busy;
} MapTable_t;


module ResvStation #(
    parameter ResvIdx = 0
) (
    // Input
    clk, rstn,

    // Master control
    instrTag,
    instrValid,
    instrType,
    instrFuncCode,

    rdIdx,
    rdType,
    hasRd,

    rdTypeOut,
    hasRdOut,

    r0Idx, r1Idx, r2Idx, 
    r0Type, r1Type, r2Type,
    regCount,
    hasImm,
    imm,

    // // RF req
    // reqMasterI, reqMasterF,
    // reqR0Idx, reqR1Idx, reqR2Idx,
    // reqR0Valid, reqR1Valid, reqR2Valid,
    // reqR0Type, reqR1Type, reqR2Type,

    // // RF bus
    // RFValidI,
    // RFdataI,
    // RFtagI,

    // RFValidF,
    // RFdataF,
    // RFtagF,

    // // CDB
    // CDBValidI,
    // CDBdataI,
    // CDBtagI,

    // CDBValidF,
    // CDBdataF,
    // CDBtagF,

    // Ready to dispatch
    Ready,
    funcUnit,
    funcCode,
    // r0Val, r1Val, r2Val, 
    vrdIdxOut,

    // Dispatch Ack
    dispatchAck,
    dispatched
);
    input clk, rstn;

    // From decode unit
    input [$clog2(`RSV_CAPACITY) - 1 : 0] instrTag;
    input instrValid;
    input FuncUnitType_t instrType;
    input FuncCode_t instrFuncCode;

    input VRegIdx_t r0Idx, r1Idx, r2Idx, rdIdx;
    input r0Type, r1Type, r2Type, rdType;
    input hasRd;

    input [1 : 0] regCount;
    input hasImm;
    input [`DWIDTH - 1 : 0] imm;
    // From decode unit end

    // To execution unit, rdTypeOut not typically used
    output logic hasRdOut;
    output logic rdTypeOut;

    // // To operand collectors
    // output logic reqMasterI, reqMasterF;
    // output GRegIdx_t reqR0Idx, reqR1Idx, reqR2Idx;
    // output logic reqR0Valid, reqR1Valid, reqR2Valid;
    // output logic reqR0Type, reqR1Type, reqR2Type;

    // // Register file operand collector I
    // input RFValidI;
    // input Vector_t RFdataI;
    // input GRegIdx_t RFtagI;

    // // Register file operand collector F
    // input RFValidF;
    // input Vector_t RFdataF;
    // input GRegIdx_t RFtagF;

    // // CDB channel I
    // input CDBValidI;
    // input Vector_t CDBdataI;
    // input GRegIdx_t CDBtagI;

    // // CDB channel F
    // input CDBValidF;
    // input Vector_t CDBdataF;
    // input GRegIdx_t CDBtagF;

    // To execution unit
    output logic Ready;
    output FuncUnitType_t funcUnit;
    output FuncCode_t funcCode;
    // output Vector_t r0Val, r1Val, r2Val;
    output VRegIdx_t vrdIdxOut;


    input dispatchAck;
    // Ctrl
    output logic dispatched;

    // Internal data structures

    // Refractor: move outside this block!

    // *lint* currently mapTable[703:64] not driven!
    MapTable_t mapTable;

    // GRegIdx_t m_r0Idx, m_r1Idx, m_r2Idx, m_rdIdx;
    // Vector_t m_r0Val, m_r1Val, m_r2Val;

    // logic m_r0Type, m_r1Type, m_r2Type, m_rdType; 

    // assign grdIdxOut = m_rdIdx;
    // assign {reqR0Type, reqR1Type, reqR2Type} = {m_r0Type, m_r1Type, m_r2Type};

    // InstrType_t m_instrType;
    // FuncCode_t m_funcCode;

    // logic needCDBr0, needCDBr1, needCDBr2;

    logic m_reqR0Valid, m_reqR1Valid, m_reqR2Valid;

    // assign reqR0Idx = m_r0Idx;
    // assign reqR1Idx = m_r1Idx;
    // assign reqR2Idx = m_r2Idx;

    // assign r0Val = m_r0Val;
    // assign r1Val = m_r1Val;
    // assign r2Val = m_r2Val;

    // assign funcType = m_instrType;
    // assign funcCode = m_funcCode;

    assign {reqR0Valid, reqR1Valid, reqR2Valid} = {m_reqR0Valid, m_reqR1Valid, m_reqR2Valid} & ~({needCDBr0, needCDBr1, needCDBr2});
   
    // logic reqMaster;
    // assign reqMaster = reqR0Valid | reqR1Valid | reqR2Valid;

    // assign reqMasterI = reqMaster && (reqR0Type == 0 || reqR1Type == 0 || reqR2Type == 0);
    // assign reqMasterF = reqMaster && (reqR0Type == 1 || reqR1Type == 1 || reqR2Type == 1);

    // mapTable.busy[vrdIdxOut] == 0 to avoid WAW
    assign Ready = ({m_reqR0Valid, m_reqR1Valid, m_reqR2Valid} == 3'b000) && dispatched == 0 && mapTable.busy[vrdIdxOut] == 0;

    always_ff @(posedge clk) begin
        if(rstn) begin
            dispatched <= 1;
            {m_reqR0Valid, m_reqR1Valid, m_reqR2Valid} <= 3'b0;
            // {needCDBr0, needCDBr1, needCDBr2} <= 3'b0;
            for (int i = 0; i < `MAX_REG_THREAD; i++) begin
                mapTable.busy <= 0;
            end
        end
        else begin
            if(instrValid && instrTag == ResvIdx) begin
                funcUnit <= instrType;
                funcCode <= instrFuncCode;
                dispatched <= 0;

                {reqR0Type, reqR1Type, reqR2Type, rdTypeOut} <= {r0Type, r1Type, r2Type, rdType};

                hasRdOut <= hasRd;
                rdTypeOut <= rdType;

                if (hasRd) begin
                    mapTable.busy[rdIdx] <= 1;
                    vrdIdxOut <= rdIdx;
                end
                reqR0Idx <= mapTable.Vreg2Greg[r0Idx];
                reqR1Idx <= mapTable.Vreg2Greg[r1Idx];
                reqR2Idx <= mapTable.Vreg2Greg[r2Idx];

                // needCDBr0 <= mapTable.busy[r0Idx] == 1;
                // needCDBr1 <= mapTable.busy[r1Idx] == 1;
                // needCDBr2 <= mapTable.busy[r2Idx] == 1;

                unique case (regCount)
                    'd0: begin
                        {m_reqR0Valid, m_reqR1Valid, m_reqR2Valid} <= 3'b000;
                        if (hasImm) begin
                            m_r0Val <= {`VEC_WIDTH{imm}};
                        end
                    end
                    'd1: begin
                        {m_reqR0Valid, m_reqR1Valid, m_reqR2Valid} <= 3'b100;
                        if (hasImm) begin
                            m_r1Val <= {`VEC_WIDTH{imm}};
                        end
                    end
                    'd2: begin
                        {m_reqR0Valid, m_reqR1Valid, m_reqR2Valid} <= 3'b110;
                        if (hasImm) begin
                            m_r2Val <= {`VEC_WIDTH{imm}};
                        end
                    end
                    'd3: begin
                        {m_reqR0Valid, m_reqR1Valid, m_reqR2Valid} <= 3'b111;
                    end
                    
                endcase
            end
            else if (dispatchAck == 1) begin
                dispatched <= 1;
            end
            else begin
                if (RFValidI /*&& reqMaster*/) begin
                    if (RFtagI == reqR0Idx && !needCDBr0 && reqR0Type == 0) begin
                        m_r0Val <= RFdataI;
                        m_reqR0Valid <= 0;
                    end
                    if (RFtagI == reqR1Idx && !needCDBr1 && reqR1Type == 0) begin
                        m_r1Val <= RFdataI;
                        m_reqR1Valid <= 0;
                    end
                    if (RFtagI == reqR2Idx && !needCDBr2 && reqR2Type == 0) begin
                        m_r2Val <= RFdataI;
                        m_reqR2Valid <= 0;
                    end
                end
                if (RFValidF) begin
                    if (RFtagF == reqR0Idx && !needCDBr0 && reqR0Type == 1) begin
                        m_r0Val <= RFdataF;
                        m_reqR0Valid <= 0;
                    end
                    if (RFtagF == reqR1Idx && !needCDBr1 && reqR2Type == 1) begin
                        m_r1Val <= RFdataF;
                        m_reqR1Valid <= 0;
                    end
                    if (RFtagF == reqR2Idx && !needCDBr2 && reqR2Type == 1) begin
                        m_r2Val <= RFdataF;
                        m_reqR2Valid <= 0;
                    end
                end
            end
        end
    end
endmodule