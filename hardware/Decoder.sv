`include "ISA.svh"
`include "gDefine.svh"

module Decoder #(
    // parameter
)(
    clk, rstn,

    // Instr tag
    tagIn, tagOut,

    // Input instruction
    instr, instrValid,

    // Output
    funcType,       // int, fp, mem, ctrl, etc
    funcCode,       // funcCode
    ra, rb, rc, rd, 
    rat, rbt, rct, rdt,
    
    // output valid
    hasDest, regCountI, regCountF,

    // output imm
    immValid,
    imm,

    // output valid
    decodeValid
    // not control

);
    input clk, rstn;

    input [$clog2(`RSV_CAPACITY) - 1 : 0] tagIn;
    output logic [$clog2(`RSV_CAPACITY) - 1 : 0] tagOut;

    input Instruction_t instr;
    input instrValid;

    output FuncUnitType_t funcType;
    output FuncCode_t funcCode;
    output logic [`REG_BITS - 1 : 0] rd, ra, rb, rc;
    output logic rat, rbt, rct, rdt;
    output logic hasDest;
    output logic [1 : 0] regCountI, regCountF;

    // Remember to sign extend when needed
    output logic immValid;
    output logic [`DWIDTH - 1 : 0] imm;

    output logic decodeValid;

    /****** Stage 1: Instruction type ******/

    logic [$clog2(`RSV_CAPACITY) - 1 : 0] tagS1;
    logic [2:0] instrTypeS1;
    logic decodeValidS1;
    Instruction_t instrS1;
    

    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            tagS1 <= {$clog2(`RSV_CAPACITY){1'b0}};
            instrTypeS1 <= 3'b111;
            decodeValidS1 <= 0;
            instrS1 <= 32'b0;
            
        end
        else begin
            if (instrValid) begin
                tagS1 <= tagIn;
                decodeValidS1 <= 1;
                instrS1 <= instr;

                unique case (instr.instrType)
                    RTYPE:      instrTypeS1 <= RTYPE;
                    RFTYPE:     instrTypeS1 <= RFTYPE;
                    ITYPE:      instrTypeS1 <= ITYPE;
                    STYPE:      instrTypeS1 <= STYPE;
                    SBTYPE:     instrTypeS1 <= SBTYPE;
                    UTYPE:      instrTypeS1 <= UTYPE;
                    UJTYPE:     instrTypeS1 <= UJTYPE;
                    default:    instrTypeS1 <= 3'b111;
                endcase
            end
            else begin
                decodeValidS1 <= 0;
            end
        end
    end
    
    /****** Stage 2: Fill the rest! ******/

    //logic [2:0] instrTypeS2;
    //logic decodeValidS2;

    always_ff @(posedge clk) begin
        if (rstn == 0) begin
            tagOut <= {$clog2(`RSV_CAPACITY){1'b0}};

            {rat, rbt, rct, rdt} <= 4'b0000;

            funcType <= 3'b0;
            funcCode <= 8'b0;
            hasDest <= 0;
            regCountI <= 0;
            regCountF <= 0;

            ra <= 0;
            rb <= 0;
            rc <= 0;
            
            rd <= 0;

            immValid <= 0;
            decodeValid <= 0;

        end
        else begin
            //decodeValidS2 <= decodeValidS1;
            //instrS2 <= instr;
            tagOut <= tagS1;
            decodeValid <= decodeValidS1;

            unique case (instrTypeS1)
                RTYPE: begin
                    funcType <= instrS1.rInst.func[4] == 1 ? FUNC_FP : FUNC_INT;
                    {rat, rbt, rct, rdt} <= instrS1.rInst.func[4] == 1 ? 4'b1111 : 4'b0000;
                    funcCode <= {instrTypeS1, instrS1.rInst.func};
                    rd <= instrS1.rInst.rd;
                    ra <= instrS1.rInst.ra;
                    rb <= instrS1.rInst.rb;
                    hasDest <= 1;

                    regCountI <= 'd2;
                    regCountF <= 0;

                end
                RFTYPE: begin
                    funcType <= FUNC_FP;
                    {rat, rbt, rct, rdt} <= 4'b1111;
                    funcCode <= {instrTypeS1, instrS1.rfInst.func};
                    rd <= instrS1.rfInst.rd;
                    ra <= instrS1.rfInst.ra;
                    rb <= instrS1.rfInst.rb;
                    rc <= instrS1.rfInst.rc;
                    hasDest <= 1;

                    regCountI <= 0;
                    regCountF <= 'd3;

                end
                ITYPE: begin
                    if (instrS1.iInst.func[3 : 2] == 2'b11) begin
                        funcType <= FUNC_FP;                // To handle int / fp conversion
                        if (instrS1.iInst.func[0] == 1'b0) begin  // Int 2 float
                            {rat, rbt, rct, rdt} <= 4'b0001;
                            regCountI <= 'd1;
                            regCountF <= 0;
                        end
                        else begin
                            {rat, rbt, rct, rdt} <= 4'b1000;
                            regCountI <= 0;
                            regCountF <= 'd1;
                        end
                    end
                    else if (instrS1.iInst.func[4 : 2] == 3'b101) begin
                        // SYS instructions
                        funcType <= FUNC_INT;
                        {rat, rbt, rct, rdt} <= 4'b0000;
                        regCountI <= 0;
                        regCountF <= 0;
                    end
                    else if (instrS1.iInst.func[4] == 1'b1) begin
                        // Mem instructions
                        funcType <= FUNC_MEM;
                        regCountI <= 1;
                        regCountF <= 0;
                        if(instrS1.iInst.func[1] == 1'b0)
                            {rat, rbt, rct, rdt} <= 4'b0000;
                        else 
                            {rat, rbt, rct, rdt} <= 4'b0001;
                    end
                    else begin
                        // Integer imm instructions
                        regCountI <= 1;
                        regCountF <= 0;
                        funcType <= FUNC_INT;
                        {rat, rbt, rct, rdt} <= 4'b0000;
                    end

                    funcCode <= {instrTypeS1, instrS1.iInst.func};
                    rd <= instrS1.iInst.rd;
                    ra <= instrS1.iInst.ra;
                    hasDest <= 1;

                    if (instrS1.iInst.func == OP_ADD || instrS1.iInst.func == OP_SUB)
                        imm <= {{20{instrS1.iInst.imm12[11]}}, instrS1.iInst.imm12};
                    else 
                        imm <= {{20{1'b0}}, instrS1.iInst.imm12};
                    immValid <= 1;
                end
                STYPE: begin
                    funcType <= FUNC_MEM;
                    if(instrS1.sInst.func[1] == 1'b0) begin
                        {rat, rbt, rct, rdt} <= 4'b0000;
                        regCountI <= 2;
                        regCountF <= 0;
                    end
                    else begin
                        {rat, rbt, rct, rdt} <= 4'b0100;
                        regCountI <= 1;
                        regCountF <= 1;
                    end
                    funcCode <= {instrTypeS1, instrS1.sInst.func};
                    ra <= instrS1.sInst.ra;
                    rb <= instrS1.sInst.rb;
                    hasDest <= 0;

                    imm <= {{20{instrS1.sInst.imm6hi[5]}}, instrS1.sInst.imm6hi, instrS1.sInst.imm6lo};
                    immValid <= 1;
                end
                SBTYPE: begin
                    funcType <= FUNC_CTRL;
                    {rat, rbt, rct, rdt} <= 4'b0000;
                    funcCode <= {instrTypeS1, instrS1.sbInst.func};
                    // Use only ra as predicate here
                    ra <= instrS1.sbInst.ra;

                    // Need to use rb as 32 bit jmp?
                    // rb <= instrS1.sbInst.rb;
                    hasDest <= 0;
                    regCountI <= 1;
                    regCountF <= 0;

                    imm <= {{20{instrS1.sbInst.imm6hi[5]}}, instrS1.sbInst.imm6hi, instrS1.sbInst.imm6lo};
                    immValid <= 1;
                end
                UTYPE: begin
                    funcType <= FUNC_INT;
                    if(instrS1.uInst.func[1] == 1'b0)
                        {rat, rbt, rct, rdt} <= 4'b0000;
                    else
                        {rat, rbt, rct, rdt} <= 4'b0001;
                    funcCode <= {instrTypeS1, {2{1'b0}}, instrS1.uInst.func};

                    rd <= instrS1.uInst.rd;

                    hasDest <= 1;
                    regCountI <= 0;
                    regCountF <= 0;
                    
                    imm <= {{12{1'b0}}, instrS1.uInst.imm20};
                    immValid <= 1;
                end
                UJTYPE: begin
                    funcType <= FUNC_CTRL;
                    {rat, rbt, rct, rdt} <= 4'b0000;
                    funcCode <= {instrTypeS1, {2{1'b0}}, instrS1.ujInst.func};
                    
                    rd <= instrS1.ujInst.rd;
                    
                    hasDest <= 0;
                    regCountI <= 0;
                    regCountF <= 0;
                    
                    imm <= {{12{1'b0}}, instrS1.ujInst.imm20};
                    immValid <= 1;
                end
                default: begin
                    
                end
            endcase
        end
    end

endmodule