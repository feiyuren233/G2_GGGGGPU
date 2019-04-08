`include "gDefine.svh"

module WriteBack #(
    parameter REQ_CHANNELS = 2
)(
    //clk, rstn,

    // Requests
    reqs,
    reqVRegIdx,
    reqDataVecs,
    reqRID,


    // Stall logic
    stallVec,

    // Grant
    //grant_oh,

    // To RF
    RFwAddr,
    RFwen,
    RFwData,

    // To Scoreboard
    wbValid, wbRID, wbVreg

    // To CDB
    // CDBValid,
    // CDBTag,
    // CDBData
);
    //input clk, rstn;

    input [REQ_CHANNELS - 1 : 0] reqs;
    input Vector_t [REQ_CHANNELS - 1 : 0] reqVRegIdx;
    input Vector_t [REQ_CHANNELS - 1 : 0] reqDataVecs;
    input RsvID_t [REQ_CHANNELS - 1 : 0] reqRID;

    output logic [REQ_CHANNELS - 1 : 0] stallVec;

    /* output */
    logic [REQ_CHANNELS - 1 : 0] grant_oh;

    output GRegIdx_t RFwAddr;
    output logic RFwen;
    output Vector_t RFwData;

    output logic wbValid;
    output RsvID_t wbRID;
    output VRegIdx_t wbVreg;

    // output logic CDBValid;
    // output GRegIdx_t CDBTag;
    // output Vector_t CDBData;

    assign stallVec = empty ? {REQ_CHANNELS{1'b0}} : ~grant_oh;
    assign wbValid = !empty;

    assign wbRID = reqRID[grant];
    assign wbVreg = reqVRegIdx[grant];

    assign RFwen = |reqs;
    // assign CDBValid = |reqs;

    logic [$clog2(REQ_CHANNELS) - 1 : 0] grant;
    logic empty;

    priArbiter #(REQ_CHANNELS) arb(reqs, grant, grant_oh, empty);

    assign RFwData = reqDataVecs[grant];
    // assign CDBData = selectedDataVec;

    assign RFwAddr = reqRID[grant] * 64 + reqVRegIdx[grant];
    // assign CDBTag = reqTag[grant];

endmodule