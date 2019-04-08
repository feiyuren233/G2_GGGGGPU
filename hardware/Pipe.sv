`include "gDefine.svh"

import gDefine::*;

module Pipe #(
    parameter QueueDepth = 1
)(
    clk, rstn,
    stall,

    // Input end
    iOpValid,
    iMask,
    iDstRegValid,
    iDstRegVID,
    iDstRegType,
    iRID,

    oOpValid,
    oMask,
    oDstRegValid,
    oDstRegVID,
    oDstRegType,
    oRID

    // // Dep query
    // queryRID,
    // qra, qrb, qrc,
    // busy

);
    input clk, rstn, stall;

    input iOpValid;
    input Mask_t iMask;
    input iDstRegValid;
    input VRegIdx_t iDstRegVID;
    input iDstRegType;
    input RsvID_t iRID;

    output logic oOpValid;
    output Mask_t oMask;
    output logic oDstRegValid;
    output VRegIdx_t oDstRegVID;
    output logic oDstRegType;
    output RsvID_t oRID;

    // input RsvID_t queryRID;
    // input VRegIdx_t qra, qrb, qrc;
    // output logic busy;

    logic [QueueDepth - 1 : 0] qOpValid;
    Mask_t [QueueDepth - 1 : 0] qMask;
    logic [QueueDepth - 1 : 0] qDstRegValid;
    VRegIdx_t [QueueDepth - 1 : 0] qDstRegVID;
    logic [QueueDepth - 1 : 0] qDstRegType;
    RsvID_t [QueueDepth - 1 : 0] qRID;

    //logic [QueueDepth - 1 : 0] query;
    //assign busy = |query;

    assign oOpValid = qOpValid[QueueDepth-1];
    assign oMask = qMask[QueueDepth-1];
    assign oDstRegValid = qDstRegValid[QueueDepth-1];
    assign oDstRegVID = qDstRegVID[QueueDepth-1];
    assign oDstRegType = qDstRegType[QueueDepth-1];
    assign oRID = qRID[QueueDepth-1];

    always_ff @(posedge clk) begin
        if(rstn == 0) begin
            for (int i = 0; i < QueueDepth; i++) begin
                qOpValid[i] <= 0;
                qDstRegValid[i] <= 0;
            end
        end
        else if (!stall) begin
            qOpValid[0] <= iOpValid;
            qMask[0] <= iMask;
            qDstRegValid[0] <= iDstRegValid;
            qDstRegVID[0] <= iDstRegVID;
            qDstRegType[0] <= iDstRegType;
            qRID[0] <= iRID;

            for (int i = 1; i < QueueDepth; i++) begin
                qOpValid[i] <= qOpValid[i - 1];
                qMask[i] <= qMask[i - 1];
                qDstRegValid[i] <= qDstRegValid[i - 1];
                qDstRegVID[i] <= qDstRegVID[i - 1];
                qDstRegType[i] <= qDstRegType[i - 1];
                qRID[i] <= qRID[i - 1];
            end
        end
    end

    // always_comb begin
    //     for (int i = 0; i < QueueDepth; i++) begin
    //         if (qDstRegValid[i] && queryRID == qRID[i] &&
    //             (
    //             qDstRegVID[i] == qra ||
    //             qDstRegVID[i] == qrb ||
    //             qDstRegVID[i] == qrc
    //             )
    //         ) 
    //         begin
    //             query[i] = 1;
    //         end
    //         else begin
    //             query[i] = 0;
    //         end
    //     end
    // end

endmodule