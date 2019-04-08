`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/06/2019 08:22:10 PM
// Design Name: 
// Module Name: RegFile
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////
`include "gDefine.svh"
import gDefine::*;

module RamP3#(
    parameter SIZE = 2048,
    parameter SIZE_L = $clog2(SIZE)
)(
    input clk,
    input [SIZE_L - 1:0] addr0,
    input [SIZE_L - 1:0] addr1,
    input [SIZE_L - 1:0] addr2,
    input [SIZE_L - 1:0] waddr,
    input Vector_t din,
    input Mask_t strb,
    input wen,
    output Vector_t d0, d1, d2
);
    (* ram_style = "block" *) Vector_t ram [SIZE - 1 : 0];
    always @ (posedge clk) begin
        if(wen) begin
            for (int i = 0; i < `VEC_WIDTH; i++) begin
                if (strb[i] == 1'b1) begin
                    ram[waddr][i] <= din[i];
                end
                else begin
                    ram[waddr][i] <= ram[waddr][i];
                end
            end
        end
        d0 <= ram[addr0];
        d1 <= ram[addr1];
        d2 <= ram[addr2];
    end

endmodule