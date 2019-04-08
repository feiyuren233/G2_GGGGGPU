`include "gDefine.svh"

module InstrIOQueue #(
    parameter QUEUE_DEPTH = 32
)(
    clk, rstn,

    // Input tag, pcAddr
    inValid,
    inTag, inAddr,

    // output tag, instr
    outTag, instr

    

);
    
endmodule