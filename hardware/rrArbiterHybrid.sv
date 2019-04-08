
module rrArbiterHybird #(
    parameter WIDTH = 16
)
(
    clk, reset,
    req,

    //output grant
    grant,
    grant_oh,
    empty
);
    localparam COUNTW = $clog2(WIDTH);

    input clk, reset;
    input [WIDTH - 1 : 0] req;
    
    output logic [COUNTW - 1 : 0] grant;
    output logic [WIDTH - 1 : 0] grant_oh;
    output logic empty;

    logic [COUNTW - 1 : 0] counter;

    logic [COUNTW - 1 : 0] ctz, nextCount;

    wire [WIDTH - 1 : 0] reqShifted = req >> counter | req << (WIDTH - counter);

    assign nextCount = (reqShifted[WIDTH - 1 : 1] != 0) ? counter + ctz : counter;

    always @ (posedge clk) begin
        if(reset == 0) begin
            counter <= 0;
        end
        counter <= nextCount;
    end

    count_trail_zero #(.W_IN(WIDTH)) ins  (
        .in(reqShifted & ~(1)),
        .out(ctz)
    );

    assign grant = counter;
    assign grant_oh = ((req & (1 << counter)) != 0) ? 1 << counter : 0;
    assign empty = (req[grant] == 0);

endmodule