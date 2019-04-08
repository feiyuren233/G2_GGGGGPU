from enum import Enum


class InstructionType(Enum):
    RTYPE = 0b000
    RFTYPE = 0b001
    ITYPE = 0b010
    STYPE = 0b011
    SBTYPE = 0b100
    UTYPE = 0b101
    UJTYPE = 0b110


class RtypeFnCode(Enum):
    # Integer pipeline
    OP_OR = 0b00000             # Bitwise logical or
    OP_AND = 0b00001            # bitwise logical and
    OP_XOR = 0b00010            # bitwise logical exclusive or
    OP_ADD = 0b00011            # Add integer
    OP_SUB = 0b00100            # Subtract integer
    OP_MUL = 0b00101            # Multiply integer low
    OP_CMPEQ_I = 0b00110        # Integer equal
    OP_CMPNE_I = 0b00111        # Integer not equal
    OP_CMPGT_I = 0b01000        # Integer greater (signed)
    OP_CMPGE_I = 0b01001        # Integer greater or equal (signed)
    OP_CMPLT_I = 0b01010        # Integer less than (signed)
    OP_CMPLE_I = 0b01011        # Integer less than or equal (signed)
    OP_CMPGT_U = 0b01100        # Integer greater than (unsigned)
    OP_CMPGE_U = 0b01101        # Integer greater or equal (unsigned)
    OP_CMPLT_U = 0b01110        # Integer less than (unsigned)
    OP_CMPLE_U = 0b01111        # Integer less than or equal (unsigned)

    # FP pineline
    OP_ADD_F = 0b10000          # Floating point add
    OP_SUB_F = 0b10001          # Floating point substract
    OP_MUL_F = 0b10010          # Floating point multiply
    OP_DIV_F = 0b10011          # Floating point divide

    OP_CMPGT_F = 0b11000        # Floating point greater than
    OP_CMPLT_F = 0b11001        # Floating point less than
    OP_CMPGE_F = 0b11010        # Floating point greater or equal
    OP_CMPLE_F = 0b11011        # Floating point less than or equal
    OP_CMPEQ_F = 0b11100        # Floating point equal
    OP_CMPNE_F = 0b11101        # Floating point not-equal


class RFtypeFnCode(Enum):
    # FP pipeline
    OP_FMA_ADD = 0b00000        # FMA a * b + c
    OP_FMA_SUB = 0b00001        # FMA a * b - c


class ItypeFnCode(Enum):
    # Integer pipeline
    OP_ASHR = 0b00000           # Arithmetic shift right (sign extend)
    OP_SHR = 0b00001            # Logical shift right (no sign extend)
    OP_SHL = 0b00010            # Logical shift left
    OP_CLZ = 0b00100            # Count leading zeroes
    OP_CTZ = 0b00101            # Count trailing zeroes
    OP_ADDI = 0b00110
    OP_SUBI = 0b00111
    OP_ADDIU = 0b01000
    OP_SUBIU = 0b01001
    OP_MULI = 0b01010

    # WARNING! NOT IMPLEMENTED
    OP_RECIPROCAL = 0b01011     # Reciprocal estimate

    # Mixed pipeline
    OP_ITOF = 0b01100           # Integer to float
    OP_FTOI = 0b01101           # Float to integer

    # Mem pipeline
    MEMOP_LOAD_I = 0b10000      # Regular load integer
    MEMOP_LL_I = 0b10001        # Load linked integer
    MEMOP_LOAD_F = 0b10010      # Regular load float
    MEMOP_LL_F = 0b10011        # Load linked float

    # System functions
    SYS_BLOCK_DIM = 0b10100     # blockDim
    SYS_BLOCK_IDX = 0b10101     # blockIdx
    SYS_THREAD_IDX = 0b10110    # threadIdx


class StypeFnCode(Enum):
    # Mem pipeline
    MEMOP_STORE_I = 0b00000     # Regular store int
    MEMOP_SC_I = 0b00001        # Store cond. int
    MEMOP_STORE_F = 0b00010     # Regular store float
    MEMOP_SC_F = 0b00011        # Store cond. float


class SBtypeFnCode(Enum):
    IF = 0b00000                # If
    ELSE = 0b00001              # Else if
    ELIF = 0b00010              # Else
    ENDIF = 0b00011             # Endif
    EXIT = 0b11111              # Exit


class UtypeFnCode(Enum):
    OP_LUI_I = 0b000            # Load upper immediate 20 I
    OP_LLI_I = 0b001            # Load lower immediate 20 I
    OP_LUI_F = 0b010            # Load upper immediate 20 F
    OP_LLI_F = 0b011            # Load lower immediate 20 F


class UJtypeFnCode(Enum):
    OP_JMP = 0b000              # Load upper immediate 20


def get_rtype_fncode_string(fncode):
    lookup = {
        RtypeFnCode.OP_OR: "or",
        RtypeFnCode.OP_AND: "and",
        RtypeFnCode.OP_XOR: "xor",
        RtypeFnCode.OP_ADD: "add",
        RtypeFnCode.OP_SUB: "sub",
        RtypeFnCode.OP_MUL: "mul",
        RtypeFnCode.OP_CMPEQ_I: "cmpeq_i",
        RtypeFnCode.OP_CMPNE_I: "cmpne_i",
        RtypeFnCode.OP_CMPGT_I: "cmpgt_i",
        RtypeFnCode.OP_CMPGE_I: "cmpge_i",
        RtypeFnCode.OP_CMPLT_I: "cmplt_i",
        RtypeFnCode.OP_CMPLE_I: "cmple_i",
        RtypeFnCode.OP_CMPGT_U: "cmpgt_u",
        RtypeFnCode.OP_CMPGE_U: "cmpge_u",
        RtypeFnCode.OP_CMPLT_U: "cmplt_u",
        RtypeFnCode.OP_CMPLE_U: "cmple_u",
        RtypeFnCode.OP_ADD_F: "add_f",
        RtypeFnCode.OP_SUB_F: "sub_f",
        RtypeFnCode.OP_MUL_F: "mul_f",
        RtypeFnCode.OP_DIV_F: "div_f",
        RtypeFnCode.OP_CMPEQ_F: "cmpeq_f",
        RtypeFnCode.OP_CMPNE_F: "cmpne_f",
        RtypeFnCode.OP_CMPGT_F: "cmpgt_f",
        RtypeFnCode.OP_CMPGE_F: "cmpge_f",
        RtypeFnCode.OP_CMPLT_F: "cmplt_f",
        RtypeFnCode.OP_CMPLE_F: "cmple_f"
    }

    return lookup[fncode]


def get_rftype_fncode_string(fncode):
    lookup = {
        RFtypeFnCode.OP_FMA_ADD: "fma_add",
        RFtypeFnCode.OP_FMA_SUB: "fma_sub"}

    return lookup[fncode]


def get_itype_fncode_string(fncode):
    lookup = {
        ItypeFnCode.OP_ASHR: "ashr",
        ItypeFnCode.OP_SHR: "shr",
        ItypeFnCode.OP_SHL: "shl",
        ItypeFnCode.OP_CLZ: "clz",
        ItypeFnCode.OP_CTZ: "ctz",
        ItypeFnCode.OP_ADDI: "addi",
        ItypeFnCode.OP_SUBI: "subi",
        ItypeFnCode.OP_ADDIU: "addiu",
        ItypeFnCode.OP_SUBIU: "subiu",
        ItypeFnCode.OP_MULI: "muli",
        ItypeFnCode.OP_RECIPROCAL: "rcp",
        ItypeFnCode.OP_ITOF: "itof",
        ItypeFnCode.OP_FTOI: "ftoi",
        ItypeFnCode.MEMOP_LOAD_I: "ld_i",
        ItypeFnCode.MEMOP_LL_I: "ll_i",
        ItypeFnCode.MEMOP_LOAD_F: "ld_f",
        ItypeFnCode.MEMOP_LL_F: "ll_f",
        ItypeFnCode.SYS_BLOCK_DIM: "blkdim",
        ItypeFnCode.SYS_BLOCK_IDX: "blkidx",
        ItypeFnCode.SYS_THREAD_IDX: "tidx"}

    return lookup[fncode]


def get_stype_fncode_string(fncode):
    lookup = {
        StypeFnCode.MEMOP_STORE_I: "st_i",
        StypeFnCode.MEMOP_SC_I: "sc_i",
        StypeFnCode.MEMOP_STORE_F: "st_f",
        StypeFnCode.MEMOP_SC_F: "sc_f"}

    return lookup[fncode]


def get_sbtype_fncode_string(fncode):
    lookup = {
        SBtypeFnCode.IF: "if",
        SBtypeFnCode.ELSE: "else",
        SBtypeFnCode.ELIF: "elif",
        SBtypeFnCode.ENDIF: "endif",
        SBtypeFnCode.EXIT: "exit"}

    return lookup[fncode]


def get_utype_fncode_string(fncode):
    lookup = {
        UtypeFnCode.OP_LUI_I: "lui_i",
        UtypeFnCode.OP_LLI_I: "lli_i",
        UtypeFnCode.OP_LUI_F: "lui_f",
        UtypeFnCode.OP_LLI_F: "lli_f"}

    return lookup[fncode]


def get_ujtype_fncode_string(fncode):
    lookup = {
        UJtypeFnCode.OP_JMP: "jmp"
    }

    return lookup[fncode]


def get_fncode_string(fncode):
    if isinstance(fncode, RtypeFnCode):
        return get_rtype_fncode_string(fncode)
    elif isinstance(fncode, RFtypeFnCode):
        return get_rftype_fncode_string(fncode)
    elif isinstance(fncode, ItypeFnCode):
        return get_itype_fncode_string(fncode)
    elif isinstance(fncode, StypeFnCode):
        return get_stype_fncode_string(fncode)
    elif isinstance(fncode, SBtypeFnCode):
        return get_sbtype_fncode_string(fncode)
    elif isinstance(fncode, UtypeFnCode):
        return get_utype_fncode_string(fncode)
    elif isinstance(fncode, UJtypeFnCode):
        return get_ujtype_fncode_string(fncode)
    else:
        raise ValueError(
            'This line contains unsupported instruction: ' + fncode)


def is_imm_floating_point_itype(fncode):
    return fncode == ItypeFnCode.OP_FTOI or fncode == ItypeFnCode.MEMOP_LOAD_F or fncode == ItypeFnCode.MEMOP_LL_F


def is_imm_floating_point_stype(fncode):
    return fncode == StypeFnCode.MEMOP_STORE_F or fncode == StypeFnCode.MEMOP_SC_F


def is_imm_floating_point_utype(fncode):
    return fncode == UtypeFnCode.OP_LUI_F or fncode == UtypeFnCode.OP_LLI_F


def is_imm_floating_point(fncode):
    if isinstance(fncode, ItypeFnCode):
        return is_imm_floating_point_itype(fncode)
    elif isinstance(fncode, StypeFnCode):
        return is_imm_floating_point_stype(fncode)
    elif isinstance(fncode, SBtypeFnCode):
        return False
    elif isinstance(fncode, UtypeFnCode):
        return is_imm_floating_point_utype(fncode)
    elif isinstance(fncode, UJtypeFnCode):
        return False
    else:
        return False
