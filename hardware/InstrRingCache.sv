`include "gDefine.svh"

import gDefine::*;



module InstrRingCache #(
    parameter NUM_ENTRIES = 32,
    parameter RAM_SIZE = 1024
)(
    clk, rstn,

    // From context manager
    reqValid,
    reqAddr,
    
    // To IO interconnect
    rd_req,
    rd_addr,
    rd_len,

    // From IO interconnect
    rd_data,
    rd_valid,
    rd_eof,

    //Hit miss
    hit,
    data

);
    localparam WordsPerEntry = RAM_SIZE / NUM_ENTRIES;
    localparam EntryBits = $clog2(NUM_ENTRIES);

    localparam TagSize = DWIDTH - $clog2(NUM_ENTRIES);
    typedef logic [TagSize - 1 : 0] Tag_t;


    input clk, rstn;

    input reqValid;
    input reqAddr;

    output logic rd_req;
    output Word_t rd_addr;
    output [11 : 0] rd_len;

    input Word_t rd_data;
    input rd_valid;
    input rd_eof;

    output logic hit;
    output Word_t data;


    logic [EntryBits - 1 : 0] head, tail;

    Tag_t [NUM_ENTRIES - 1 : 0] Tags;
    logic [NUM_ENTRIES - 1 : 0] Valid, Pending;

    logic [NUM_ENTRIES - 1 : 0] hitVec;

    assign hit = |hitVec;





    





endmodule