`include "gDefine.svh"
import gDefine::*;

module InstrMemSim #(
    
)(
    clk, rstn,

    reqs,
    reqPCAddrVec,

    // Output 
    pcAck,
    instrValid,
    tag,
    instr

);
    input clk, rstn;
    input [`RSV_CAPACITY - 1 : 0] reqs;
    input Word_t [`RSV_CAPACITY - 1 : 0] reqPCAddrVec;

    output [`RSV_CAPACITY - 1 : 0] pcAck;
    output logic instrValid;
    output [`RSV_BITS - 1 : 0] tag;
    output Word_t instr;

    Word_t mem [2047 : 0];
    initial begin
          $readmemb("hardware/asm.mem", mem);
    end

    logic [`RSV_CAPACITY - 1 : 0] grantOH;
    logic [$clog2(`RSV_CAPACITY) - 1 : 0] grant;
    logic empty;

    rrArbiterHybird #(`RSV_CAPACITY) arb(
        clk, rstn,
        reqs,
        grant,
        grantOH,
        empty
    );

    always_comb begin
        instr = mem[reqPCAddrVec[grant] >> 2];
        pcAck = grantOH;
        tag = grant;
        instrValid = !empty;
    end


    
endmodule