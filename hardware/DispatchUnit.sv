`include "gDefine.svh"
import gDefine::*;

module DispatchUnit #(
    parameter DispatcherType = 3'b000
)(
    clk, rstn,

    // Stall from pipeline
    stall,

    // Input region
    opValid,
    opIn,
    funcUnitType, 
    ra, rb, rc,
    rd, rdt, rdValid,

    // Input region END

    // Output port
    opValidOut,
    funcCode,

    // Output dispatch ack signal
    dispatchAck,

    // Operands to function unit
    a, b, c,

    // Output logical regs
    dstRegValid, 
    dstReg, 
    dstRegType

);
    input clk, rstn;
    input stall;

    input [`RSV_CAPACITY - 1 : 0] opValid;
    input FuncCode_t [`RSV_CAPACITY - 1 : 0] opIn;
    input FuncUnitType_t [`RSV_CAPACITY - 1 : 0] funcUnitType;

    input VRegIdx_t [`RSV_CAPACITY - 1 : 0] ra, rb, rc;

    input [`RSV_CAPACITY - 1 : 0] rdValid;
    input VRegIdx_t [`RSV_CAPACITY - 1 : 0] rd;
    input [`RSV_CAPACITY - 1 : 0] rdt;

    output logic opValidOut;
    output FuncCode_t funcCode;

    output logic [`RSV_CAPACITY - 1 : 0] dispatchAck;

    output Vector_t a, b, c;


    output logic dstRegValid;
    output VRegIdx_t dstReg;
    output logic dstRegType;


    /***** Arbiter *****/

    logic [`RSV_CAPACITY - 1 : 0] dispatchValid;

    genvar i;
    generate;
        for (i = 0; i < RSV_CAPACITY; i++) begin
            assign dispatchValid = funcUnitType[i] == DispatcherType && opValid[i];
        end
    endgenerate

    logic [$clog2(`RSV_CAPACITY) - 1 : 0] grant;
    logic [`RSV_CAPACITY - 1 : 0] grant_oh;
    logic empty;

    rrArbiterHybird #(RSV_CAPACITY) arb(
        clk, rstn,
        dispatchValid,
        grant,
        grant_oh,
        empty
    );

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

            
            funcCode = opIn[grant];

        end
    end

    
endmodule