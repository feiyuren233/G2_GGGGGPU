`include "gDefine.svh"
import gDefine::*;

module ContextManager #(
    parameter ctxID = 0
)(
    clk, rstn,

    // to IO interconnect
    reqPCValid,
    PCout,

    // // to exec unit
    // maskOut,

    // from IO interconnect
    pcAck,

    // from idk
    unfreeze,

    // from ctrl unit
    // maskOP,
    // newMask,
    // busTag,

    // From core control
    loadReq,
    loadPC,

    blockID,
    blockDim,
    warpID,

    // Output
    available,
    
    // To exec units
    blockIDOut,
    blockDimOut,
    warpIDOut,

    // Probably never used
    nextPCValid,
    nextPC

);
    input clk, rstn;

    output logic reqPCValid;
    output Word_t PCout;
    
    // output Mask_t maskOut;

    input pcAck;

    input unfreeze;
    // input [1 : 0] maskOP;
    // input Mask_t newMask;

    // Used to listen on bus
    // input [`RSV_BITS - 1 : 0] busTag;

    input loadReq;
    input Word_t loadPC;

    input BlockID_t blockID;
    input BlockDim_t blockDim;
    input WarpID_t warpID;

    output logic available;

    output BlockID_t blockIDOut;
    output BlockDim_t blockDimOut;
    output WarpID_t warpIDOut;

    input nextPCValid;
    input Word_t nextPC;

    // Mask_t [15 : 0] maskStack;
    // logic [3 : 0] maskTop;
    logic pcIncrFlag;

    // Event seq:
    // loadReq -> load PC
//3 // ReqIO
    // pcAck -> req enqueued


    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            reqPCValid <= 0;
            if (ctxID < 32) begin
                available <= 1;
            end
            else 
                available <= 0;
            pcIncrFlag <= 0;
        end
        else begin
            if (loadReq) begin
                PCout <= loadPC;
                reqPCValid <= 1;
                available <= 0;
                pcIncrFlag <= 0;

                blockIDOut <= blockID;
                blockDimOut <= blockDim;
                warpIDOut <= warpID;

            end
            else if (pcAck) begin
                // reqPCValid <= 0;
                PCout <= PCout + 4;
                reqPCValid <= 0;
            end
            else if (unfreeze && !available) begin
                reqPCValid <= 1;
            end

            // Next pc logic
            // if (nextPCValid && ctxID == busTag) begin
            //     PCout <= nextPC;
            //     pcIncrFlag <= 1;
            // end
        end
        
    end



endmodule