`include "gDefine.svh"
module OperandCollector #(
    parameter ReqVecWidth = 16,
    parameter CollectorType = 0
)(
    clk, rstn,

    // To Reg file read port
    RFrAddr,
    RFrData,

    // Input from reservation station

    reqMaster,
    reqGIdx,
    reqGIdxValid,
    reqGIdxType,

    // Output, RF Bus
    RFBusTag,
    RFBusValid,
    RFBusData

);
    input clk, rstn;

    output GRegIdx_t RFrAddr;
    input Vector_t RFrData;

    input [ReqVecWidth - 1 : 0] reqMaster;
    input GRegIdx_t [ReqVecWidth - 1 : 0] [2 : 0] reqGIdx;
    input [ReqVecWidth - 1 : 0] [2 : 0] reqGIdxValid;
    input [ReqVecWidth - 1 : 0] [2 : 0] reqGIdxType;



    output GRegIdx_t RFBusTag;
    output logic RFBusValid;
    output Vector_t RFBusData;

    logic [$clog2(ReqVecWidth) - 1 : 0] grant;
    logic empty;

    rrArbiterDec #(ReqVecWidth) arbiter (clk, rstn, reqMaster, grant, empty);


    // Relavent signals
    GRegIdx_t [2 : 0] mReqGIdx;
    logic [2 : 0] mReqGIdxValid;
    logic [2 : 0] mReqGIdxType;

    assign mReqGIdx = reqGIdx[grant];
    assign mReqGIdxValid = reqGIdxValid[grant];
    assign mReqGIdxType = reqGIdxType[grant];


    assign RFBusData = RFrData;

    /****** Stage 1: See if any valid requests exist ******/

    logic RFBusValidS1;
    GRegIdx_t RFBusTagS1;

    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            RFBusValidS1 <= 0;
        end
        else begin
            if (!empty) begin
                if (mReqGIdxValid[0] == 1 && mReqGIdxType[0] == CollectorType) begin
                    RFBusValidS1 <= 1;
                    RFBusTagS1 <= mReqGIdx[0];
                    RFrAddr <= mReqGIdx[0];
                end
                else if (mReqGIdxValid[1] == 1 && mReqGIdxType[1] == CollectorType) begin
                    RFBusValidS1 <= 1;
                    RFBusTagS1 <= mReqGIdx[1];
                    RFrAddr <= mReqGIdx[1];
                end
                else if (mReqGIdxValid[2] == 1 && mReqGIdxType[2] == CollectorType) begin
                    RFBusValidS1 <= 1;
                    RFBusTagS1 <= mReqGIdx[2];
                    RFrAddr <= mReqGIdx[2];
                end
                else begin
                    RFBusValidS1 <= 0;
                end
            end
            else begin
                RFBusValidS1 <= 0;
            end
        end
    end

    /****** Stage 2: RF got addr, wait ******/

    logic RFBusValidS2;
    GRegIdx_t RFBusTagS2;

    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            RFBusValidS2 <= 0;
        end
        else begin
            RFBusValidS2 <= RFBusValidS1;
            RFBusTagS2 <= RFBusTagS1;
        end
    end

    /****** Stage 3: Data finally arrives ******/

    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            RFBusValid <= 0;
        end
        else begin
            RFBusValid <= RFBusValidS2;
            RFBusTag <= RFBusTagS2;
        end
    end

endmodule