`ifndef GDEFINE_H
`define GDEFINE_H



// Latency defination
`define FP_LATENCY (21)

// 2^6 = 64 regs max
`define REG_BITS (6)
// `define FP_REG_BITS (6)

// Word width
`define DWIDTH (32)

// `define FWORD real
`define FWORD logic [`DWIDTH - 1:0]

`define VEC_WIDTH (16)

`define MAX_RESV (32)

`define MAX_REG_THREAD (64)

`define OP_CODE_BITS (5)

`define RSV_CAPACITY (32)

`define RSV_BITS (5)

`define RF_REG_BITS (11)

`define WB_QUEUE_DEPTH (16)

`define SCOREBOARD_SIZE (32)


package gDefine;

// Types

typedef logic [`DWIDTH - 1 : 0] Scalar_t;
typedef Scalar_t [`VEC_WIDTH - 1 : 0] Vector_t;

typedef logic [2 : 0] InstrType_t;
typedef logic [7 : 0] FuncCode_t;

typedef logic [`REG_BITS - 1 : 0] VRegIdx_t;
typedef logic [`RF_REG_BITS - 1 : 0] GRegIdx_t;
typedef logic [`DWIDTH - 1 : 0] Word_t;
typedef logic [`VEC_WIDTH - 1 : 0] Mask_t;
typedef logic [$clog2(`SCOREBOARD_SIZE) - 1 : 0] RsvID_t;

// Context management specific
typedef logic [8 : 0] BlockID_t;
typedef logic [9 : 0] BlockDim_t;
typedef logic [4 : 0] WarpID_t;


// Enums

typedef enum logic [2 : 0] {
    FUNC_INT    = 3'b000,
    FUNC_FP     = 3'b001,
    FUNC_MEM    = 3'b010,
    FUNC_CTRL   = 3'b011

} FuncUnitType_t;

// ???
typedef enum logic [1 : 0] {
    MASK_OP_PUSH    = 2'b00


} MaskOP_t;

endpackage : gDefine

`endif