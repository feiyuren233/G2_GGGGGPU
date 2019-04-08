`include "gDefine.svh"
module dram#(
    parameter ADDR_WIDTH = 14,
    parameter BURST_WIDTH = 8
)(
    clk, rst,
    //Read side
    rAddr, arValid, arReady,
    arLen, 
    rData, rValid, rReady,
    rLast,

    //Write side
    wAddr, awValid, awReady,
    awLen,
    wData, wValid, wReady,
    wStrb,
    wLast
);
    input clk, rst;

    input [ADDR_WIDTH - 1 : 0] rAddr;
    input arValid;
    output logic arReady;
    input [BURST_WIDTH - 1 : 0] arLen;
    output logic [`DWIDTH - 1 : 0] rData;
    output logic rValid;
    input rReady;
    output rLast;

    input [ADDR_WIDTH - 1 : 0] wAddr;
    input awValid;
    output logic awReady;
    input [BURST_WIDTH - 1 : 0] awLen; 
    input [`DWIDTH - 1 : 0] wData;
    input wValid;
    output logic wReady;
    input logic [3:0] wStrb;
    input wLast;

    reg [`DWIDTH - 1 : 0] memory [2**ADDR_WIDTH - 1 : 0];
    
    initial begin
          $readmemh("hardware/hex_memory_file.mem", memory);
    end
  

    enum integer {
        IDLE = 0,
        TRANSFER = 1
    } STATE;

    logic [0 : 0] rState, rNextState;
    logic [BURST_WIDTH - 1 : 0] rburstCount, rburstRequested;

    logic [ADDR_WIDTH - 1 : 0] mrAddr; 

    always_comb begin
        case(rState)
            IDLE: begin
                if(arValid) rNextState = TRANSFER;
            end

            TRANSFER: begin
                if(rburstCount == rburstRequested) rNextState = IDLE;
            end
            default: rNextState = IDLE;
        endcase
    end


    always_ff @(posedge clk) begin
        if(rst == 0) begin
            //Reset shits
            rState <= IDLE;
        end
        else begin
            rState <= rNextState;
            case(rState)
                IDLE: begin
                    arReady <= 0;
                    //rValid <= 0;
                    rburstCount <= 0;
                    rburstRequested <= arLen;
                    mrAddr <= rAddr >> 2;

                    if(arValid) begin
                        arReady <= 1;
                        rValid <= 1;
                    end
                end

                TRANSFER: begin
                    arReady <= 0;
                    rValid <= 1;

                    /* verilator lint_off WIDTH */
                    //rData <= memory[mrAddr + rburstCount];
                    /* verilator lint_on WIDTH */
                    
                    if(rReady) rburstCount <= rburstCount + 1;
                    if(rLast) rValid <= 0;

                end
                default: begin
                end
            endcase
        end
    end

    /* verilator lint_off WIDTH */
    assign rData = memory[mrAddr + rburstCount];
    /* verilator lint_on WIDTH */

    assign rLast = rburstCount == rburstRequested;


    logic [0 : 0] wState, wNextState;
    logic [BURST_WIDTH - 1 : 0] wburstCount, wburstRequested;

    logic [ADDR_WIDTH - 1 : 0] mwAddr; 

    wire canWrite = (awValid) || (wburstRequested != 0 && wburstCount <= wburstRequested);

    always_ff @ (posedge clk) begin
        if(rst == 0) begin
            wburstRequested <= 0;
            wburstCount <= 0;
        end
        else begin
            awReady <= 0;
            if(canWrite) begin
                /* verilator lint_off WIDTH */
                for(int i = 0; i < 4; i++) begin
                    memory[mwAddr + wburstCount][8 * i +: 8] <= wStrb[i] ? wData[8 * i +: 8] : memory[mwAddr + wburstCount][8 * i +: 8];
                end
                /* verilator lint_on WIDTH */
                wburstCount <= wburstCount + 1;
                wReady <= 1;
            end
            if(awValid && wburstRequested == 0) begin
                wburstRequested <= awLen;
                awReady <= 1;
            end
            else if(wburstCount == wburstRequested) begin
                wburstCount <= 0;
            end

        end
    end 

    // always_comb begin
    //     case(wState)
    //         IDLE: begin
    //             if(awValid) wNextState = TRANSFER;
    //         end

    //         TRANSFER: begin
    //             if(wburstCount == wburstRequested) wNextState = IDLE;
    //         end
    //         default: wNextState = IDLE;
    //     endcase
    // end

    // always_ff @(posedge clk) begin
    //     if(rst == 0) begin
    //         wState <= IDLE;
    //     end
    //     else begin
    //         wState <= wNextState;
    //         case(wState)
    //             IDLE: begin
    //                 awReady <= 0;
    //                 wReady <= 0;
    //                 wburstCount <= 0;
    //                 wburstRequested <= awLen;
    //                 mwAddr <= wAddr >> 2;
    //                 if(awValid && wValid) begin
    //                     /* verilator lint_off WIDTH */
    //                     for(int i = 0; i < 4; i++) begin
    //                         memory[mwAddr + wburstCount][8 * i +: 8] <= wStrb[i] ? wData[8 * i +: 8] : memory[mwAddr + wburstCount][8 * i +: 8];
    //                     end
    //                     wburstCount <= wburstCount + 1;
    //                     /* verilator lint_on WIDTH */
    //                     wReady <= 1;
    //                 end
    //                 if(awValid) begin
    //                     awReady <= 1;
    //                 end
    //             end
    //             TRANSFER: begin
    //                 awReady <= 0; 
    //                 if(wValid && wburstCount <= wburstRequested) begin
    //                     /* verilator lint_off WIDTH */
    //                     for(int i = 0; i < 4; i++) begin
    //                         memory[mwAddr + wburstCount][8 * i +: 8] <= wStrb[i] ? wData[8 * i +: 8] : memory[mwAddr + wburstCount][8 * i +: 8];
    //                     end
    //                     /* verilator lint_on WIDTH */
    //                     wburstCount <= wburstCount + 1;
    //                     wReady <= 1;
    //                 end 

    //             end
    //             default: begin
    //             end
    //         endcase
    //     end
    // end

    // always_ff @ (posedge clk) begin
    //     if(rst == 0) awReady <= 0;
    //     else begin
    //         if(awValid) awReady <= 1;
    //         if(wState == TRANSFER) awReady <= 0;
    //     end
    // end

    // always_ff @ (posedge clk) begin
    //     if(wValid || (wburstCount != wburstRequested && wState == TRANSFER)) begin
    //         /* verilator lint_off WIDTH */
    //         for(int i = 0; i < 4; i++) begin
    //             memory[mwAddr + wburstCount][8 * i +: 8] <= wStrb[i] ? wData[8 * i +: 8] : memory[mwAddr + wburstCount][8 * i +: 8];
    //         end
    //         wReady <= 1;
    //         /* verilator lint_on WIDTH */
    //     end
    //     else begin
    //         wReady <= 0;
    //     end
    // end

endmodule