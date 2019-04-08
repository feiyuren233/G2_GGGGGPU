`include "gDefine.svh"

import gDefine::*;

module StoreUnit #(
)(
    clk, rstn,

    // From dispatch
    storeValid,
    storeAddr,
    storeData,
    storeMask,

    // To dispatch
    storeReady,

    // To axi master
    initWrite,
    writeAddress,
    writeLen,

    wDataValid,
    wData,
    mask,

    // From axi master
    wDataNext,
    wDone
);
    input clk, rstn;

    input storeValid;
    input Vector_t storeAddr;
    input Vector_t storeData;
    input Mask_t storeMask;

    output logic storeReady;
    
    output logic initWrite;
    output Word_t writeAddress;
    output [7 : 0] writeLen;

    output logic wDataValid;
    output Word_t wData;
    output [3 : 0] mask;

    input wDataNext;
    input wDone;


    Vector_t dataCopy;
    Vector_t addrCopy; 
    Mask_t maskCopy;


    logic bubble;
    logic [3 : 0] counter;
    // Always valid in this dumb store unit
    assign wDataValid = 1;
    // Always do 16 long burst in this dumb store unit
    assign writeLen = 'd16;

    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            initWrite <= 0;
            storeReady <= 1;
            bubble <= 0;
            counter <= 0;
        end
        else if (storeValid & storeReady) begin
            storeReady <= 0;
            bubble <= 1;
            
            maskCopy <= storeMask;
            // Safe?
            initWrite <= 1;
            counter <= 0;
        end
        else if (bubble == 1) begin
            dataCopy <= storeData;
            addrCopy <= storeAddr;

            wData <= storeData[0];
            writeAddress <= storeAddr[0];
            // mask <= {4{maskCopy[0]}};

            bubble <= 0;
        end

        else if (wDataNext) begin
            wData <= dataCopy[counter];
            // mask <= {4{maskCopy[counter]}};
            counter <= counter + 1;
        end

        if (wDone) begin
            initWrite <= 0;
            storeReady <= 1;
        end
        
    end

    assign mask = {4{maskCopy[counter]}};
    
endmodule