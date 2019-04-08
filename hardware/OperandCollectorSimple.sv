`include "gDefine.svh"
import gDefine::*;


module OperandCollectorSimple #(
    // parameter
)(
    clk, rstn,

    ra, rb, rc,
    rat, rbt, rct,

    rfAddrI0, rfAddrI1, rfAddrF0, rfAddrF1, rfAddrF2
);
    always_comb begin
        if (rat == 0) 
            rfAddrI0 = ra;
        else
            rfAddrF0 = ra;
    end

    
endmodule