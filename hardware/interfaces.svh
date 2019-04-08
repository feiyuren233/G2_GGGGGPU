`ifndef INTERFACES_H
`define INTERFACES_H

`include "gDefine.svh"


interface IFMA (
    input wire clk, clken, rstn,
    input wire mode,
    input `FWORD a,
    input `FWORD b,
    input `FWORD c,
    output `FWORD out
);

endinterface


`endif