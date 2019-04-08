`include "gDefine.svh"
import gDefine::*;

module DispatchUnitSimple #(
)(
    clk, rstn,

    // Stall from pipeline
    stall,

    // Input region
    opValid,
    opIn,
    funcUnitType, 
    ra, rb, rc,
    rat, rbt, rct,
    imm,
    rd, rdt, rdValid,

    // Input region END

    // Output port
    opValidOut,
    funcUnitTypeOut,
    funcCode,

    // Output dispatch ack signal
    dispatchAck,

    // Operands to function unit
    a, b, c,
    at, bt, ct,
    immOut,

    // Output logical regs
    dstRegValid, 
    dstReg, 
    dstRegType,

    rid

);
    input clk, rstn;
    input stall;

    input [`RSV_CAPACITY - 1 : 0] opValid;
    input FuncCode_t [`RSV_CAPACITY - 1 : 0] opIn;
    input FuncUnitType_t [`RSV_CAPACITY - 1 : 0] funcUnitType;

    input VRegIdx_t [`RSV_CAPACITY - 1 : 0] ra, rb, rc;
    input [`RSV_CAPACITY - 1 : 0] rat, rbt, rct;

    input Word_t [`RSV_CAPACITY - 1 : 0] imm;

    input [`RSV_CAPACITY - 1 : 0] rdValid;
    input VRegIdx_t [`RSV_CAPACITY - 1 : 0] rd;
    input [`RSV_CAPACITY - 1 : 0] rdt;

    output logic opValidOut;
    output FuncUnitType_t funcUnitTypeOut;
    output FuncCode_t funcCode;

    output logic [`RSV_CAPACITY - 1 : 0] dispatchAck;

    output VRegIdx_t a, b, c;
    output logic at, bt, ct;
    output Word_t immOut;


    output logic dstRegValid;
    output VRegIdx_t dstReg;
    output logic dstRegType;

    output RsvID_t rid;


    /***** Arbiter *****/

    logic [`RSV_CAPACITY - 1 : 0] dispatchValid;

    genvar i;
    generate;
        for (i = 0; i < `RSV_CAPACITY; i++) begin
            assign dispatchValid[i] = opValid[i];
        end
    endgenerate

    logic [$clog2(`RSV_CAPACITY) - 1 : 0] grant;
    logic [`RSV_CAPACITY - 1 : 0] grant_oh;
    logic empty;

    rrArbiterHybird #(`RSV_CAPACITY) arb(
        clk, rstn,
        dispatchValid,
        grant,
        grant_oh,
        empty
    );


    // Output logic
    always_comb begin
        if (rstn == 0) begin
            opValidOut = 0;
            dispatchAck = 0;
        end
        else if (stall || empty) begin
            opValidOut = 0;
            dispatchAck = 0;
        end
        else begin
            opValidOut = 1;
            dispatchAck = grant_oh;

            funcUnitTypeOut = funcUnitType[grant];

            a = ra[grant];
            b = rb[grant];
            c = rc[grant];
            at = rat[grant];
            bt = rbt[grant];
            ct = rct[grant];

            rid = grant;

            immOut = imm[grant];
            
            funcCode = opIn[grant];

            dstRegValid = rdValid[grant];
            dstReg = rd[grant];
            dstRegType = rdt[grant];

        end
    end

    
endmodule