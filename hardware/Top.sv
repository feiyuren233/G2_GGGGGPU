`include "gDefine.svh"
import gDefine::*;

/* verilator lint_off LITENDIAN */
module Top #(
    parameter  C_M_TARGET_SLAVE_BASE_ADDR	= 32'h00000000,
    // Burst Length. Supports 1, 2, 4, 8, 16, 32, 64, 128, 256 burst lengths
    // parameter integer C_M_AXI_BURST_LEN	= 64,
    // Thread ID Width
    parameter integer C_M_AXI_ID_WIDTH	= 1,
    // Width of Address Bus
    parameter integer C_M_AXI_ADDR_WIDTH	= 32,
    // Width of Data Bus
    parameter integer C_M_AXI_DATA_WIDTH	= 32,
    // Width of User Write Address Bus
    parameter integer C_M_AXI_AWUSER_WIDTH	= 0,
    // Width of User Read Address Bus
    parameter integer C_M_AXI_ARUSER_WIDTH	= 0,
    // Width of User Write Data Bus
    parameter integer C_M_AXI_WUSER_WIDTH	= 0,
    // Width of User Read Data Bus
    parameter integer C_M_AXI_RUSER_WIDTH	= 0,
    // Width of User Response Bus
    parameter integer C_M_AXI_BUSER_WIDTH	= 0
)(
    clk, rstn,

    reqValid,
    reqBlockDim,
    reqWarpCount,
    reqPC,

    // Output done
    goodJob,

    // Interrupt clear
    clearEcho,

    // AXI full interface
        output wire [C_M_AXI_ID_WIDTH-1 : 0] M_AXI_AWID,
		// Master Interface Write Address
		output wire [C_M_AXI_ADDR_WIDTH-1 : 0] M_AXI_AWADDR,
		// Burst length. The burst length gives the exact number of transfers in a burst
		output wire [7 : 0] M_AXI_AWLEN,
		// Burst size. This signal indicates the size of each transfer in the burst
		output wire [2 : 0] M_AXI_AWSIZE,
		// Burst type. The burst type and the size information, 
    // determine how the address for each transfer within the burst is calculated.
		output wire [1 : 0] M_AXI_AWBURST,
		// Lock type. Provides additional information about the
    // atomic characteristics of the transfer.
		output wire  M_AXI_AWLOCK,
		// Memory type. This signal indicates how transactions
    // are required to progress through a system.
		output wire [3 : 0] M_AXI_AWCACHE,
		// Protection type. This signal indicates the privilege
    // and security level of the transaction, and whether
    // the transaction is a data access or an instruction access.
		output wire [2 : 0] M_AXI_AWPROT,
		// Quality of Service, QoS identifier sent for each write transaction.
		output wire [3 : 0] M_AXI_AWQOS,
		// Optional User-defined signal in the write address channel.
		output wire [C_M_AXI_AWUSER_WIDTH-1 : 0] M_AXI_AWUSER,
		// Write address valid. This signal indicates that
    // the channel is signaling valid write address and control information.
		output wire  M_AXI_AWVALID,
		// Write address ready. This signal indicates that
    // the slave is ready to accept an address and associated control signals
		input wire  M_AXI_AWREADY,
		// Master Interface Write Data.
		output wire [C_M_AXI_DATA_WIDTH-1 : 0] M_AXI_WDATA,
		// Write strobes. This signal indicates which byte
    // lanes hold valid data. There is one write strobe
    // bit for each eight bits of the write data bus.
		output wire [C_M_AXI_DATA_WIDTH/8-1 : 0] M_AXI_WSTRB,
		// Write last. This signal indicates the last transfer in a write burst.
		output wire  M_AXI_WLAST,
		// Optional User-defined signal in the write data channel.
		output wire [C_M_AXI_WUSER_WIDTH-1 : 0] M_AXI_WUSER,
		// Write valid. This signal indicates that valid write
    // data and strobes are available
		output wire  M_AXI_WVALID,
		// Write ready. This signal indicates that the slave
    // can accept the write data.
		input wire  M_AXI_WREADY,
		// Master Interface Write Response.
		input wire [C_M_AXI_ID_WIDTH-1 : 0] M_AXI_BID,
		// Write response. This signal indicates the status of the write transaction.
		input wire [1 : 0] M_AXI_BRESP,
		// Optional User-defined signal in the write response channel
		input wire [C_M_AXI_BUSER_WIDTH-1 : 0] M_AXI_BUSER,
		// Write response valid. This signal indicates that the
    // channel is signaling a valid write response.
		input wire  M_AXI_BVALID,
		// Response ready. This signal indicates that the master
    // can accept a write response.
		output wire  M_AXI_BREADY,
		// Master Interface Read Address.
		output wire [C_M_AXI_ID_WIDTH-1 : 0] M_AXI_ARID,
		// Read address. This signal indicates the initial
    // address of a read burst transaction.
		output wire [C_M_AXI_ADDR_WIDTH-1 : 0] M_AXI_ARADDR,
		// Burst length. The burst length gives the exact number of transfers in a burst
		output wire [7 : 0] M_AXI_ARLEN,
		// Burst size. This signal indicates the size of each transfer in the burst
		output wire [2 : 0] M_AXI_ARSIZE,
		// Burst type. The burst type and the size information, 
    // determine how the address for each transfer within the burst is calculated.
		output wire [1 : 0] M_AXI_ARBURST,
		// Lock type. Provides additional information about the
    // atomic characteristics of the transfer.
		output wire  M_AXI_ARLOCK,
		// Memory type. This signal indicates how transactions
    // are required to progress through a system.
		output wire [3 : 0] M_AXI_ARCACHE,
		// Protection type. This signal indicates the privilege
    // and security level of the transaction, and whether
    // the transaction is a data access or an instruction access.
		output wire [2 : 0] M_AXI_ARPROT,
		// Quality of Service, QoS identifier sent for each read transaction
		output wire [3 : 0] M_AXI_ARQOS,
		// Optional User-defined signal in the read address channel.
		output wire [C_M_AXI_ARUSER_WIDTH-1 : 0] M_AXI_ARUSER,
		// Write address valid. This signal indicates that
    // the channel is signaling valid read address and control information
		output wire  M_AXI_ARVALID,
		// Read address ready. This signal indicates that
    // the slave is ready to accept an address and associated control signals
		input wire  M_AXI_ARREADY,
		// Read ID tag. This signal is the identification tag
    // for the read data group of signals generated by the slave.
		input wire [C_M_AXI_ID_WIDTH-1 : 0] M_AXI_RID,
		// Master Read Data
		input wire [C_M_AXI_DATA_WIDTH-1 : 0] M_AXI_RDATA,
		// Read response. This signal indicates the status of the read transfer
		input wire [1 : 0] M_AXI_RRESP,
		// Read last. This signal indicates the last transfer in a read burst
		input wire  M_AXI_RLAST,
		// Optional User-defined signal in the read address channel.
		input wire [C_M_AXI_RUSER_WIDTH-1 : 0] M_AXI_RUSER,
		// Read valid. This signal indicates that the channel
    // is signaling the required read data.
		input wire  M_AXI_RVALID,
		// Read ready. This signal indicates that the master can
    // accept the read data and response information.
		output wire  M_AXI_RREADY,

        // Debug interface
        output logic isStore,
        output Vector_t storeAddr,
        output Vector_t storeData,
        output Mask_t storeMask
);

    input clk, rstn;
    input reqValid;
    input [9 : 0] reqBlockDim;
    input [5 : 0] reqWarpCount;

    input Word_t reqPC;

    output logic goodJob;

    input clearEcho;

    // ReqValid edge gen
    logic reqValidf, reqValidff;
    logic reqValidEdge;

    always_ff @(posedge clk) begin
        reqValidf <= reqValid;
        reqValidff <= reqValidf;
    end

    assign reqValidEdge = reqValidf & !reqValidff;

    // stu logic
    logic initWrite;
    Word_t writeAddr;
    logic [7 : 0] writeLen;

    logic wDataValid;
    Word_t wData;
    logic [3 : 0] mask;

    logic wDataNext;
    logic wDone;


    // Core control logic
    logic [5 : 0] resvCount;
    logic submitWarp;

    logic [9 : 0] m_reqBlockDim;
    logic [5 : 0] m_reqWarpCount; 
    Word_t m_reqPC;

    logic [9 : 0] c_reqBlockDim;
    logic [5 : 0] c_reqWarpCount; 
    /* verilator lint_off PINMISSING */
    Core core0 (
        clk, rstn,
        resvCount,
        submitWarp,
        m_reqPC,
        c_reqBlockDim[8 : 0],
        {4'b000, m_reqWarpCount} << 4,
        c_reqWarpCount[4 : 0],

        initWrite,
        writeAddr,
        writeLen,

        wDataValid,
        wData,
        mask,

        wDataNext,
        wDone,

        // Debug
        isStore,
        storeAddr,
        storeData,
        storeMask
    );
    /* verilator lint_on PINMISSING */

    AXIMaster axiMaster(
        initWrite,
        writeAddr,
        writeLen,

        wDataValid,
        wData,
        mask,

        wDataNext,
        wDone,

        clk, rstn,
        .*
    );

    // Master logic


    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            m_reqBlockDim <= 0;
            m_reqWarpCount <= 0;
            m_reqPC <= 0;
        end
        else if(reqValidEdge) begin
            m_reqBlockDim <= reqBlockDim;
            m_reqWarpCount <= reqWarpCount;
            m_reqPC <= reqPC;
        end
        else if (clearEcho) begin
            m_reqBlockDim <= 0;
            m_reqWarpCount <= 0;
        end
        else begin
            m_reqBlockDim <= m_reqBlockDim;
            m_reqWarpCount <= m_reqWarpCount;
        end
    end

    // Good job logic
    logic submitEnd, empty;
    assign submitEnd = c_reqBlockDim == m_reqBlockDim;
                

    assign empty = m_reqBlockDim == 0;

    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            goodJob <= 0;
        end
        else if (reqValidEdge || clearEcho) begin
            goodJob <= 0;
        end
        else begin
            if (submitEnd &&
                resvCount == 'd32 &&
                !empty) begin

                goodJob <= 1;
            end
        end
    end

    // Job submit logic

    logic canSubmit;
    assign canSubmit = !submitEnd && resvCount != 0 && !empty;

    assign submitWarp = canSubmit;
    // Increment logic

    always_ff @(posedge clk) begin
        if(rstn == 0) begin
            c_reqBlockDim <= 0;
            c_reqWarpCount <= 0;
        end
        else if(reqValidEdge) begin
            c_reqBlockDim <= 0;
            c_reqWarpCount <= 0;
        end
        else if (canSubmit) begin
            if(c_reqWarpCount == m_reqWarpCount - 1) 
            begin
                c_reqWarpCount <= 0;
                c_reqBlockDim <= c_reqBlockDim + 1;
            end
            else if (c_reqWarpCount != m_reqWarpCount - 1) begin
                c_reqWarpCount <= c_reqWarpCount + 1;
            end
            else begin
                c_reqWarpCount <= c_reqWarpCount;
                c_reqBlockDim <= c_reqBlockDim;
            end
        end
         
    end

endmodule