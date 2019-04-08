`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/06/2019 06:54:09 PM
// Design Name: 
// Module Name: FMA_sim
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
`include "interfaces.svh"


module FMA_sim(
    clk, clken, rst,
    a, b, c, mode,
    out
);
    input clk, clken, rst;
    input `FWORD a, b, c;
    input mode;
    output `FWORD out;

    IFMA ifma(clk, clken, rst, mode, a, b, c, out);

    FMA fmaU(
        .fma(ifma)
    );

endmodule

module FMA(
    IFMA fma
    );
parameter DELAY = 21;

`FWORD pipeline [DELAY - 1 : 0];

`FWORD mid;
assign mid = fma.a * fma.b;
`FWORD result;
assign result = fma.mode ? mid + fma.c : mid - fma.c;

always_ff @ (posedge fma.clk) begin
    if (fma.rstn == 0) begin
        for(int i = 1; i < DELAY; i++) begin
            pipeline[i] <= 0;
        end
    end
    else if(fma.clken) begin
        pipeline[0] <= result;
        for(int i = 1; i < DELAY; i++) begin
            pipeline[i] <= pipeline[i - 1];
        end
    end
end    

assign fma.out = pipeline[DELAY - 1];

endmodule

// module P3Ram(
//     input clk,
//     input [7:0] addr1,
//     input [7:0] addr2,
//     input [7:0] addr3,
//     input [7:0] waddr,
//     input [511 : 0] din,
//     input wea,
//     output logic [511 : 0] o1, o2, o3
// );
//     (* ram_style = "block" *) logic [511 : 0] ram [1023 : 0];
//     always @ (posedge clk) begin
//         if(wea) begin
//             ram[waddr] <= din;
//         end
//         o1 <= ram[addr1];
//         o2 <= ram[addr2];
//         o3 <= ram[addr3];
//     end

// endmodule


// module P3RamTest(input clk, input data, output reg [2 : 0]shit, input [7:0] a1, a2, a3, wa);
//     logic wea;
//     //input [7:0] a1, a2, a3, wa;
//     logic [511 : 0] din, o1, o2, o3;
    
//     reg [8:0] counter;
//     reg [511 : 0] realData;
    
//     always @(posedge clk) begin
//         realData <= 512'b0;
//         realData[counter] <= 1;
//         counter <= counter + 1;
//     end
    
// //    assign din = {512{data}};

// //design_1 design_1_inst
// //       (.BRAM_PORTA_0_addr(a1),
// //        .BRAM_PORTA_0_clk(clk),
// //        .BRAM_PORTA_0_din(realData),
// //        .BRAM_PORTA_0_dout(o1),
// //        .BRAM_PORTA_0_en(1),
// //        .BRAM_PORTA_0_we(1),
// //        .BRAM_PORTB_0_addr(a2),
// //        .BRAM_PORTB_0_clk(clk),
// //        .BRAM_PORTB_0_din(realData),
// //        .BRAM_PORTB_0_dout(o2),
// //        .BRAM_PORTB_0_en(1),
// //        .BRAM_PORTB_0_we(1));
    
//     P3Ram inst (clk, a1, a2, a3, wa, realData, 1, o1, o2, o3);
    

//     logic [511 : 0] r0 = o1[0];
//     logic [511 : 0] r1 = o1[1];
//     logic [511 : 0] r2 = o1[2];
    
    
//     always @ (posedge clk) begin
        
//         shit[0] <= o1[counter];
//         shit[1] <= o2[counter];
//         shit[2] <= o3[counter];
//     end
    

// endmodule