`include "gDefine.svh"

module WBQueue #(
    
)(
    clk, rstn,

    // Output, can accept
    chReadyA, chReadyB,
    // chIDA, chIDB,

    // Output evict signal
    wbEvictValid,
    wbEvictID,

    // Dual input port
    reqValidA, reqTIDA, reqVIdxA, reqDataA, reqMaskA,
    reqValidB, reqTIDB, reqVIdxB, reqDataB, reqMaskB

    // BroadCast Port

);
    
endmodule