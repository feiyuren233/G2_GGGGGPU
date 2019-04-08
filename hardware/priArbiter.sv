`include "gDefine.svh"

module priArbiter #(
    parameter Channels = 2  // power of 2
)(
    // combinational
    reqs,

    grant,
    grantOH,
    empty
);
    localparam ChannelPOW2 = 2 ** $clog2(Channels);
    localparam ChannelcLog2 = $clog2(Channels);


    input [Channels - 1 : 0] reqs;
    output [ChannelcLog2 - 1 : 0] grant;
    output [Channels - 1 : 0] grantOH;
    output empty;

    logic [ChannelcLog2 - 1 : 0] trailZero;
    count_trail_zero #(ChannelPOW2) clz (reqs, trailZero);

    assign empty = reqs == 0;
    assign grant = trailZero;
    assign grantOH = ('b1 << trailZero) & reqs;

endmodule