`ifndef ISA_H
`define ISA_H


// R type instr, most common XXX ra, rb -> rd instructions

typedef struct packed {
    logic [4 : 0] func;                     // Additional function
    logic [5 : 0] pad;                      // pad
    logic [5 : 0] ra;                       // 2 ^ 6 = 64 max ra
    logic [5 : 0] rb;                       // 2 ^ 6 = 64 max rb
    logic [5 : 0] rd;                       // 2 ^ 6 = 64 max rd
    logic [2 : 0] instType;                 // 3 bits instType
    // 3'b000
} RInst;

typedef enum logic[4:0] {
    // Integer pipeline
    OP_OR                   = 5'b00000,     // Bitwise logical or
    OP_AND                  = 5'b00001,     // bitwise logical and
    OP_XOR                  = 5'b00010,     // bitwise logical exclusive or
    OP_ADD                  = 5'b00011,     // Add integer
    OP_SUB                  = 5'b00100,     // Subtract integer
    OP_MULT                 = 5'b00101,     // Multiply integer low
    OP_CMPEQ_I              = 5'b00110,     // Integer equal
    OP_CMPNE_I              = 5'b00111,     // Integer not equal
    OP_CMPGT_I              = 5'b01000,     // Integer greater (signed)
    OP_CMPGE_I              = 5'b01001,     // Integer greater or equal (signed)
    OP_CMPLT_I              = 5'b01010,     // Integer less than (signed)
    OP_CMPLE_I              = 5'b01011,     // Integer less than or equal (signed)
    OP_CMPGT_U              = 5'b01100,     // Integer greater than (unsigned)
    OP_CMPGE_U              = 5'b01101,     // Integer greater or equal (unsigned)
    OP_CMPLT_U              = 5'b01110,     // Integer less than (unsigned)
    OP_CMPLE_U              = 5'b01111,     // Integer less than or equal (unsigned)

    // FP pineline
    OP_ADD_F                = 5'b10000,     // Floating point add
    OP_SUB_F                = 5'b10001,     // Floating point substract
    OP_MUL_F                = 5'b10010,     // Floating point multiply
    OP_DIV_F                = 5'b10011,     // Floating point divide

    OP_CMPGT_F              = 5'b11000,     // Floating point greater than
    OP_CMPLT_F              = 5'b11001,     // Floating point less than
    OP_CMPGE_F              = 5'b11010,     // Floating point greater or equal
    OP_CMPLE_F              = 5'b11011,     // Floating point less than or equal
    OP_CMPEQ_F              = 5'b11100,     // Floating point equal
    OP_CMPNE_F              = 5'b11101      // Floating point not-equal

    // Long op (Div / Mult?)
    // 5'b10100 Base

} Rtype_FnCode;

// RF type instructions, 3 operand 1 dst FMA operations!

typedef struct packed {
    logic [4 : 0] func;                     // Additional function
    logic [5 : 0] ra;                       // 2 ^ 6 = 64 max ra
    logic [5 : 0] rb;                       // 2 ^ 6 = 64 max rb
    logic [5 : 0] rc;                       // 2 ^ 6 = 64 max rc
    logic [5 : 0] rd;                       // 2 ^ 6 = 64 max rd
    logic [2 : 0] instType;                 // 3 bits instType
    // 3'b001
} RFInst;

typedef enum logic[4 : 0] {
    // FP pipeline
    OP_FMA_ADD              = 5'b00000,     // FMA a * b + c
    OP_FMA_SUB              = 5'b00001      // FMA a * b - c

} RFtype_FnCode;


// I type instructions, including single operand & imm 

typedef struct packed {
    logic [4 : 0] func;                     // Additional function
    logic [11 : 0] imm12;                   // 12 bits immediate
    logic [5 : 0] ra;                       // 2 ^ 6 = 64 max ra
    logic [5 : 0] rd;                       // 2 ^ 6 = 64 max rd
    logic [2 : 0] instType;                 // 3 bits instType
    // 3'b010
} IInst;

typedef enum logic[4 : 0] {
    // Integer pipeline
    OP_ASHR                 = 5'b00000,     // Arithmetic shift right (sign extend)
    OP_SHR                  = 5'b00001,     // Logical shift right (no sign extend)
    OP_SHL                  = 5'b00010,     // Logical shift left
    OP_CLZ                  = 5'b00100,     // Count leading zeroes
    OP_CTZ                  = 5'b00101,     // Count trailing zeroes
    OP_ADDI                 = 5'b00110,
    OP_SUBI                 = 5'b00111,
    OP_ADDIU                = 5'b01000,     
    OP_SUBIU                = 5'b01001,
    OP_MULTI                = 5'b01010,

    // WARNING! NOT IMPLEMENTED
    OP_RECIPROCAL           = 5'b01011,     // Reciprocal estimate

    // Mixed pipeline
    OP_ITOF                 = 5'b01100,     // Integer to float
    OP_FTOI                 = 5'b01101,     // Float to integer

    // Mem pipeline
    MEMOP_LOAD_I            = 5'b10000,     // Regular load integer
    MEMOP_LL_I              = 5'b10001,     // Load linked integer
    MEMOP_LOAD_F            = 5'b10010,     // Regular load float
    MEMOP_LL_F              = 5'b10011,     // Load linked float

    // System functions
    SYS_BLOCK_DIM           = 5'b10100,     // blockDim
    SYS_BLOCK_IDX           = 5'b10101,     // blockIdx
    SYS_THREAD_IDX          = 5'b10110      // threadIdx

} Itype_FnCode;

// S type instructions, store

typedef struct packed {
    logic [4 : 0] func;                     // Additional function
    logic [5 : 0] imm6hi;                   // more imm
    logic [5 : 0] rb;                       // 12 bits immediate    value
    logic [5 : 0] ra;                       // 2 ^ 6 = 64 max rb    addr
    logic [5 : 0] imm6lo;                   // imm
    logic [2 : 0] instType;                 // 3 bits instType
    // 3'b011
} SInst;

typedef enum logic[4:0] {
    // Mem pipeline
    MEMOP_STORE_I           = 5'b00000,     // Regular store int
    MEMOP_SC_I              = 5'b00001,     // Store cond. int
    MEMOP_STORE_F           = 5'b00010,     // Regular store float
    MEMOP_SC_F              = 5'b00011      // Store cond. float

} Stype_FnCode;

// SB type instruction, conditional branch

typedef struct packed {
    logic [4 : 0] func;                     // Additional function
    logic [5 : 0] imm6hi;                   // more imm
    logic [5 : 0] rb;                       // 12 bits immediate
    logic [5 : 0] ra;                       // 2 ^ 6 = 64 max rb
    logic [5 : 0] imm6lo;                   // imm
    logic [2 : 0] instType;                 // 3 bits instType
    // 3'b100
} SBInst;

typedef enum logic[4:0] {
    CTRL_IF                 = 5'b00000,     // IF cond
    CTRL_ELSE               = 5'b00001,     // ELSE
    CTRL_ELIF               = 5'b00010,     // ELSE IF cond
    CTRL_ENDIF              = 5'b00011,     // ENDIF
    EXIT                    = 5'b11111      // Exit

} SBtype_FnCode;

// U type, 20 bit imm

typedef struct packed {
    logic [2 : 0] func;                     // 3 bit function
    logic [19 : 0] imm20;                   // 20 bit immediate
    logic [5 : 0] rd;                       // 2 ^ 6 = 64 max rd
    logic [2 : 0] instType;                 // 3 bits instType
    // 3'b101
} UInst;

typedef enum logic[2:0] {
    OP_LUI_I                = 3'b000,       // Load upper immediate 20 I
    OP_LLI_I                = 3'b001,       // Load lower immediate 20 I
    OP_LUI_F                = 3'b010,       // Load upper immediate 20 F
    OP_LLI_F                = 3'b011        // Load lower immediate 20 F

} Utype_FnCode;


// UJ type, long jump unconditional

typedef struct packed {
    logic [2 : 0] func;                     // 3 bit function
    logic [19 : 0] imm20;                   // 20 bit immediate
    logic [5 : 0] rd;                       // 2 ^ 6 = 64 max rd
    logic [2 : 0] instType;                 // 3 bits instType
    // 3'b110
} UJInst;

typedef enum logic[2:0] {
    OP_JMP                  = 3'b000        // 

} UJtype_FnCode;

typedef enum logic [2 : 0] {
    RTYPE   = 3'b000,
    RFTYPE  = 3'b001,
    ITYPE   = 3'b010,
    STYPE   = 3'b011,
    SBTYPE  = 3'b100,
    UTYPE   = 3'b101,
    UJTYPE  = 3'b110

} InstrTypeCode;


typedef union packed {
    RInst   rInst;
    RFInst  rfInst;
    IInst   iInst;
    SInst   sInst;
    SBInst  sbInst;
    UInst   uInst;
    UJInst  ujInst;
    logic [2 : 0] instrType;
} Instruction_t;


`endif