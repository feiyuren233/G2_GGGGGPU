`ifndef WBBROADCAST_H
`define WBBROADCAST_H

`include "gDefine.svh"

typedef struct packed{
    //logic int_OR_fp;
    logic [`REG_BITS - 1 : 0] regID;
    logic [`DWIDTH - 1 : 0] [`VEC_WIDTH - 1 : 0] val ;

} IntWBBroadcast_t;

typedef struct packed{
    //logic int_OR_fp,
    logic [`REG_BITS - 1 : 0] regID;
    logic [`DWIDTH - 1 : 0] [`VEC_WIDTH - 1 : 0] val;

} FPWBBroadcast_t;

typedef struct packed{
    logic int_OR_fp;
    logic [`REG_BITS - 1 : 0] regID;
    logic [`DWIDTH - 1 : 0] [`VEC_WIDTH - 1 : 0] val;

} MemWBBroadcast_t;

`endif