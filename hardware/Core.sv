`include "gDefine.svh"
`include "WBBroadcast.svh"
`include "interfaces.svh"
import gDefine::*;

module Core #(
    parameter CORE_ID = 0
) (
    clk, rstn,

    // Load control
    // out
    resvStationCount,
    //regPoolCount,

    // in
    inValid,
    // reqRegCount,
    reqPC,

    blockID,
    blockDim,
    warpID,

    // To axi control interface

    initWrite,
    writeAddr,
    writeLen,

    wDataValid,
    wData,
    mask,

    wDataNext,
    wDone,

    isStore,
    storeAddr,
    storeData,
    storeMask

);  
    //Debug
    output logic isStore;
    output Vector_t storeAddr;
    output Vector_t storeData;
    output Mask_t storeMask;

    input clk, rstn;

    output logic [5 : 0] resvStationCount;
    //output logic [`RF_REG_BITS : 0] regPoolCount;

    input inValid;
    //input [5 : 0] reqRegCount;
    input Word_t reqPC; 

    input BlockID_t blockID;
    input BlockDim_t blockDim;
    input WarpID_t warpID;

    output logic initWrite;
    output Word_t writeAddr;
    output logic [7 : 0] writeLen;

    output logic wDataValid;
    output Word_t wData;
    output logic [3 : 0] mask;

    input logic wDataNext;
    input logic wDone;

    // Used later
    logic [`RSV_CAPACITY - 1 : 0] exitSigVec;

    logic [`RSV_CAPACITY - 1 : 0] rstnVec;

    logic [`RSV_CAPACITY - 1 : 0] reqPCValid;
    Word_t [`RSV_CAPACITY - 1 : 0] reqPCAddr;

    Mask_t [`RSV_CAPACITY - 1 : 0] maskOut;
    logic [`RSV_CAPACITY - 1 : 0] pcAck;
    logic [`RSV_CAPACITY - 1 : 0] unfreeze;
    MaskOP_t maskOP;
    Mask_t newMask;
    logic [`RSV_BITS - 1 : 0] busTag;

    logic [`RSV_CAPACITY - 1 : 0] loadReqOHGrant;

    logic [`RSV_CAPACITY - 1 : 0] availVec;

    logic nextPCValid;
    Word_t nextPC;

    /***** Context management *****/

    logic [`RSV_CAPACITY - 1 : 0] unfreezeDispatch;
    logic [`RSV_CAPACITY - 1 : 0] unfreezeCtrl;

    assign unfreeze = unfreezeDispatch | unfreezeCtrl;


    assign rstnVec ={`RSV_CAPACITY{rstn}} & ~exitSigVec;
    /* verilator lint_off PINMISSING */
    priArbiter #(`RSV_CAPACITY) arbCxt (.reqs(availVec), .grantOH(loadReqOHGrant));
    /* verilator lint_on PINMISSING */

    BlockID_t [`RSV_CAPACITY - 1 : 0] blockIDVec;
    BlockDim_t [`RSV_CAPACITY - 1 : 0] blockDimVec;
    WarpID_t [`RSV_CAPACITY - 1 : 0] warpIDVec;

    // To control
    popcnt #(`RSV_CAPACITY) pcnt(availVec, resvStationCount);

    genvar i;
    generate
        for(i = 0; i < `RSV_CAPACITY; i++) begin
            ContextManager #(i) ctxManager (clk, rstnVec[i], 
                reqPCValid[i],
                reqPCAddr[i],
                pcAck[i],
                unfreeze[i],
                loadReqOHGrant[i] && inValid,
                reqPC,
                blockID,
                blockDim,
                warpID,
                availVec[i],
                // IDs
                blockIDVec[i],
                blockDimVec[i],
                warpIDVec[i],
                
                // Not used for now
                nextPCValid,
                nextPC
            );

        end
    endgenerate

    /***** io interconnect *****/
    logic [$clog2(`RSV_CAPACITY) - 1 : 0] tag2Decode;
    Word_t instrIn;
    logic instrValid;

    InstrMemSim instrMem(
        clk, rstn,
        reqPCValid,
        reqPCAddr,
        pcAck,
        instrValid,
        tag2Decode,
        instrIn
    );

    /***** Decode unit *****/
    
    logic [$clog2(`RSV_CAPACITY) - 1 : 0] tagFromDecode;

    
    FuncUnitType_t funcUnitType;
    FuncCode_t funcCode;

    logic [`REG_BITS - 1 : 0] rd, ra, rb, rc;
    logic rat, rbt, rct, rdt;
    logic hasDest;
    logic [1 : 0] regCountI, regCountF;

    logic immValid;
    logic [`DWIDTH - 1 : 0] imm;

    logic decodeValid;

    Decoder decode (
        clk, rstn,
        tag2Decode,
        tagFromDecode,
        instrIn,
        instrValid,
        funcUnitType,
        funcCode,
        ra, rb, rc, rd,
        rat, rbt, rct, rdt,
        hasDest,
        regCountI, regCountF,
        immValid,
        imm,
        decodeValid
    );


    /***** Reg files *****/
    logic [10 : 0] addr0I, addr1I, wAddrI;
    Vector_t dinI;
    Mask_t strbI;
    logic wenI;
    Vector_t o0I, o1I;

    assign strbI = maskVec[wbThreadI];

    RamP2 IntReg(
        clk,
        addr0I, addr1I, wAddrI,
        dinI,
        strbI,
        wenI,
        o0I,
        o1I
    );

    logic [10 : 0] addr0F, addr1F, addr2F, wAddrF;
    Vector_t dinF;
    Mask_t strbF;
    logic wenF;
    Vector_t o0F, o1F, o2F;

    assign strbF = maskVec[wbThreadF];

    RamP3 FPReg(
        clk,
        addr0F, addr1F, addr2F, wAddrF,
        dinF,
        strbF,
        wenF,
        o0F,
        o1F,
        o2F
    );

    /***** Scoreboard *****/

    logic wbValidI, wbValidF;
    VRegIdx_t wbVregI, wbVregF;
    RsvID_t wbThreadI, wbThreadF;

    VRegIdx_t [`RSV_CAPACITY - 1 : 0] reqR0Idx, reqR1Idx, reqR2Idx;
    logic [`RSV_CAPACITY - 1 : 0] reqR0Type, reqR1Type, reqR2Type;

    
    logic [`RSV_CAPACITY - 1 : 0] reqRdType;
    logic [`RSV_CAPACITY - 1 : 0] hasRd;

    logic [`RSV_CAPACITY - 1 : 0] Ready;
    FuncUnitType_t [`RSV_CAPACITY - 1 : 0] sbFuncUnit;
    FuncCode_t [`RSV_CAPACITY - 1 : 0] sbFuncCode;

    VRegIdx_t [`RSV_CAPACITY - 1 : 0] reqRdIdx;

    logic [`RSV_CAPACITY - 1 : 0] [1 : 0] reqRegCountI, reqRegCountF;
    logic [`RSV_CAPACITY - 1 : 0] sbHasImm;
    Word_t [`RSV_CAPACITY - 1 : 0] sbImm;

    logic [`RSV_CAPACITY - 1 : 0] dispatchAck, dispatched;

    generate
        for(i = 0; i < `RSV_CAPACITY; i++) begin
            Scoreboard #(i) sb(
                clk, rstn,
                decodeValid,
                tagFromDecode,
                funcUnitType,
                funcCode,
                rd, rdt, hasDest,
                ra, rb, rc,
                rat, rbt, rct,
                regCountI, regCountF,
                immValid,
                imm,

                // WB section
                wbValidI, wbThreadI, wbVregI, 
                wbValidF ,wbThreadF, wbVregF,

                // Operand output
                reqR0Idx[i], reqR1Idx[i], reqR2Idx[i], 
                reqR0Type[i], reqR1Type[i], reqR2Type[i], 

                reqRdType[i],
                hasRd[i],

                // Ready to dispatch
                Ready[i],
                sbFuncUnit[i],
                sbFuncCode[i],
                reqRdIdx[i],

                reqRegCountI[i], reqRegCountF[i],
                sbHasImm[i],
                sbImm[i],

                dispatchAck[i],
                dispatched[i]
            );
        end
    endgenerate


    /***** Dispatch units *****/
    logic stallInt, stallFP, stallMEM;    
    logic stall;

    // Not used for now
    // ?? 
    logic DispatchValid;
    FuncUnitType_t duFuncUnit;
    FuncCode_t duFuncCode;

    VRegIdx_t duA, duB, duC;
    logic duAt, duBt, duCt;
    Word_t duImm;

    logic duDstRegValid;
    VRegIdx_t duDstRegIdx;
    logic duDstRegType;

    RsvID_t rid;

    logic storeReady;
    
    // Store blocking
    logic [`RSV_CAPACITY - 1 : 0] stReadyMask;
    always_comb begin
        for(int i = 0; i < `RSV_CAPACITY; i++) begin
            if (sbFuncUnit[i] == FUNC_MEM && !storeReady) begin
                stReadyMask[i] = 0;
            end 
            else
                stReadyMask[i] = 1;
        end
    end

    DispatchUnitSimple duSimple(
        clk, rstn,
        stall,
        Ready & stReadyMask,
        sbFuncCode,
        sbFuncUnit,
        reqR0Idx, reqR1Idx, reqR2Idx,
        reqR0Type, reqR1Type, reqR2Type,
        sbImm,
        reqRdIdx, reqRdType, hasRd,

        // Valid
        DispatchValid,
        duFuncUnit,
        duFuncCode,

        dispatchAck,

        duA, duB, duC,
        duAt, duBt, duCt,
        duImm,

        duDstRegValid,
        duDstRegIdx,
        duDstRegType,

        rid
    );

    // GRegIdx_t GduA = duA + rid * 64;
    // GRegIdx_t GduB = duB + rid * 64;
    // GRegIdx_t GduC = duC + rid * 64;

    assign addr0I = GRegIdx_t'(duA) + rid * 64;
    assign addr1I = GRegIdx_t'(duB) + rid * 64;

    assign addr0F = GRegIdx_t'(duA) + rid * 64;
    assign addr1F = GRegIdx_t'(duB) + rid * 64;
    assign addr2F = GRegIdx_t'(duC) + rid * 64;


    always_comb begin
        for(i = 0; i < `RSV_CAPACITY; i++) begin
            /* verilator lint_off WIDTH */
            if(DispatchValid && duFuncUnit != FUNC_CTRL && rid == i)
                unfreezeDispatch[i] = 1;
            else
                unfreezeDispatch[i] = 0;

            // Ignore ctrl for now

            /* verilator lint_on WIDTH */
        end
    end

    /***** Execution pipeline *****/

    Mask_t [`RSV_CAPACITY - 1 : 0] maskVec;

    Vector_t outputI, outputF;

    logic intOPValid, fpOPValid, ctrlOpValid;

    logic storeOpValid;

    assign intOPValid = duFuncUnit == FUNC_INT && DispatchValid;
    assign fpOPValid = duFuncUnit == FUNC_FP && DispatchValid;
    assign ctrlOpValid = duFuncUnit == FUNC_CTRL && DispatchValid;
    assign storeOpValid = duFuncUnit == FUNC_MEM && DispatchValid;

    logic isStoreLatch;
    Mask_t latchbla;

    logic ipValid;
    Mask_t ipMask;
    logic ipDstRegValid;
    VRegIdx_t ipDstRegVID;
    logic ipDstRegType;
    RsvID_t ipRid;

    // Compensate delay in reg file
    RsvID_t duRidLatch;
    Word_t duImmLatch;
    FuncCode_t duFuncCodeLatch;
    logic ctrlValidLatch;

    always_ff @(posedge clk) begin
        duRidLatch <= rid;
        duImmLatch <= duImm;
        duFuncCodeLatch <= duFuncCode;
        ctrlValidLatch <= ctrlOpValid;

        isStoreLatch <= storeOpValid;
        isStore <= isStoreLatch;

        storeData <= o1I;
        storeAddr <= o0I;
        latchbla <= maskVec[rid];
        storeMask <= latchbla;
    end

    Pipe #(2) intPipe(
        clk, rstn,
        stallInt,
        intOPValid,
        maskVec[rid],// Mask
        duDstRegValid,
        duDstRegIdx,
        duDstRegType,
        rid,

        ipValid,
        ipMask,
        ipDstRegValid,
        ipDstRegVID,
        ipDstRegType,
        ipRid
    );

    IntExecPipeline intExec(
        clk, rstn,
        duFuncCodeLatch,
        blockIDVec,
        blockDimVec,
        warpIDVec,
        duRidLatch,
        o0I, o1I, duImmLatch,
        outputI,
        stallInt
    );

    logic fpValid;
    Mask_t fpMask;
    logic fpDstRegValid;
    VRegIdx_t fpDstRegVID;
    logic fpDstRegType;
    RsvID_t fpRid;

    Pipe #(21 + 1) fpPipe(
        clk, rstn,
        stallFP,
        fpOPValid,
        maskVec[rid],// Mask
        duDstRegValid,
        duDstRegIdx,
        duDstRegType,
        rid,

        fpValid,
        fpMask,
        fpDstRegValid,
        fpDstRegVID,
        fpDstRegType,
        fpRid
    );

    FPExecPipeline fpExec(
        clk, rstn,

        duFuncCodeLatch,
        o0F, o1F, o2F,
        outputF,
        stallFP
    );

    BranchUnit bu(
        clk, rstn,
        maskVec,
        ctrlValidLatch,
        duFuncCodeLatch,
        duRidLatch,
        o0I,
        unfreezeCtrl,
        exitSigVec
    );


    StoreUnit stu(
        clk, rstn, 
        storeOpValid,
        o0I,
        o1I,
        maskVec[rid],

        storeReady,

        initWrite,
        writeAddr,
        writeLen,

        wDataValid,
        wData,
        mask,

        wDataNext,
        wDone
    );

    /***** Write back *****/

    WritebackSimple #(1) wbI(
        {ipValid},
        {ipDstRegVID},
        {outputI},
        {ipRid},
        {stallInt},

        // To RF
        wAddrI,
        wenI,
        dinI,

        // To Scoreboard
        wbValidI,
        wbThreadI,
        wbVregI
    );

    WritebackSimple #(1) wbF(
        {fpValid},
        {fpDstRegVID},
        {outputF},
        {fpRid},
        {stallFP},

        // To RF
        wAddrF,
        wenF,
        dinF,

        // To Scoreboard
        wbValidF,
        wbThreadF,
        wbVregF
    );

    // TODO:
    // Context management
    // ->
    // io interconnect
    // -> 
    // Decode unit
    // -> 
    // Pull up instrValid on Resv station
    // -> 
    // Resv station wait for dispatch
    // -> 
    // Dispatch Ack -> can fetch next instr if not ctrl
    // ->
    // send to exec unit (INT, FP, MEM, CTRL)
    // ->
    // write back, pull down stall in case of ctrl
    // -> 
    // Done!

endmodule