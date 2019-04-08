from isa import *
from assembler_util import *
from struct import pack, unpack


def reinterpret_fp_to_int(fp):
    tmp = pack('f', fp)
    ret = unpack('I', tmp)[0]
    print(fp, "=>", hex(ret))
    return ret


# (fncode, operands)
def generate_machine_code(instruction):
    fncode, operands = instruction

    if isinstance(fncode, RtypeFnCode):
        rd, ra, rb = operands

        rd = int(rd[1:])
        ra = int(ra[1:])
        rb = int(rb[1:])

        instType = InstructionType.RTYPE.value
        func = fncode.value
        inst_word = ((func & 0b11111) << 27) | ((ra & 0b111111) << 15) | (
            (rb & 0b111111) << 9) | ((rd & 0b111111) << 3) | (instType & 0b111)

        print(func, " ", ra, " ", rb, " ", rd, " ", instType)
        print(str(get_bin(inst_word, 32)))

        return inst_word
    elif isinstance(fncode, RFtypeFnCode):
        rd, ra, rb, rc = operands

        rd = int(rd[1:])
        ra = int(ra[1:])
        rb = int(rb[1:])
        rc = int(rc[1:])

        instType = InstructionType.RFTYPE.value
        func = fncode.value
        inst_word = ((func & 0b11111) << 27) | ((ra & 0b111111) << 21) | ((rb & 0b111111) << 15) | (
            (rc & 0b111111) << 9) | ((rd & 0b111111) << 3) | (instType & 0b111)

        print(func, " ", ra, " ", rb, " ", rc, " ", rd, " ", instType)
        print(str(get_bin(inst_word, 32)))

        return inst_word
    elif isinstance(fncode, ItypeFnCode):
        rd, ra, imm12 = operands

        rd = int(rd[1:])
        ra = int(ra[1:])

        if is_imm_floating_point(fncode):
            imm12 = reinterpret_fp_to_int(imm12)

        instType = InstructionType.ITYPE.value
        func = fncode.value
        inst_word = ((func & 0b11111) << 27) | ((imm12 & 0b111111111111) << 15) | (
            (ra & 0b111111) << 9) | ((rd & 0b111111) << 3) | (instType & 0b111)

        print(func, " ", imm12, " ", ra, " ", rd, " ", instType)
        print(str(get_bin(inst_word, 32)))

        return inst_word
    elif isinstance(fncode, StypeFnCode):
        ra, rb, imm12 = operands

        ra = int(ra[1:])
        rb = int(rb[1:])

        if is_imm_floating_point(fncode):
            imm12 = reinterpret_fp_to_int(imm12)

        instType = InstructionType.STYPE.value
        func = fncode.value
        inst_word = ((func & 0b11111) << 27) | (((imm12 & 0b111111000000) >> 6) << 21) | (
            (rb & 0b111111) << 15) | ((ra & 0b111111) << 9) | ((imm12 & 0b000000111111) << 3) | (instType & 0b111)

        print(func, " ", rb, " ", ra, " ", imm12, " ", instType)
        print(str(get_bin(inst_word, 32)))

        return inst_word
    elif isinstance(fncode, SBtypeFnCode):
        ra, rb, imm12 = operands

        ra = int(ra[1:])
        rb = int(rb[1:])

        if is_imm_floating_point(fncode):
            imm12 = reinterpret_fp_to_int(imm12)

        instType = InstructionType.SBTYPE.value
        func = fncode.value
        inst_word = ((func & 0b11111) << 27) | (((imm12 & 0b111111000000) >> 6) << 21) | (
            (rb & 0b111111) << 15) | ((ra & 0b111111) << 9) | ((imm12 & 0b000000111111) << 3) | (instType & 0b111)

        print(func, " ", ra, " ", rb, " ", imm12, " ", instType)
        print(str(get_bin(inst_word, 32)))

        return inst_word
    elif isinstance(fncode, UtypeFnCode):
        rd, imm20 = operands

        rd = int(rd[1:])

        if is_imm_floating_point(fncode):
            imm20 = reinterpret_fp_to_int(imm20)

        instType = InstructionType.UTYPE.value
        func = fncode.value
        inst_word = ((func & 0b111) << 29) | ((imm20 & 0b11111111111111111111) << 9) | (
            (rd & 0b111111) << 3) | (instType & 0b111)

        print(func, " ", imm20, " ", rd, " ",  instType)
        print(str(get_bin(inst_word, 32)))

        return inst_word
    elif isinstance(fncode, UJtypeFnCode):
        rd, imm20 = operands

        rd = int(rd[1:])

        if is_imm_floating_point(fncode):
            imm20 = reinterpret_fp_to_int(imm20)

        instType = InstructionType.UJTYPE.value
        func = fncode.value
        inst_word = ((func & 0b111) << 29) | ((imm20 & 0b11111111111111111111) << 9) | (
            (rd & 0b111111) << 3) | (instType & 0b111)

        print(func, " ", imm20, " ", rd, " ", instType)
        print(str(get_bin(inst_word, 32)))

        return inst_word
    else:
        raise ValueError(
            'FUCK! CHARLES EXPLODED!')


# (fncode, operands)
def generate_assembly_code(instruction):
    fncode, operands = instruction

    if isinstance(fncode, RtypeFnCode):
        rd, ra, rb = operands
        as_code = get_fncode_string(
            fncode) + " " + rd + ", " + ra + ", " + rb
        return as_code
    elif isinstance(fncode, RFtypeFnCode):
        rd, ra, rb, rc = operands
        as_code = get_fncode_string(
            fncode) + " " + rd + ", " + ra + ", " + rb + ", " + rc
        return as_code
    elif isinstance(fncode, ItypeFnCode):
        rd, ra, imm12 = operands
        as_code = get_fncode_string(
            fncode) + " " + rd + ", " + ra + ", $" + str(imm12)
        return as_code
    elif isinstance(fncode, StypeFnCode):
        ra, rb, imm12 = operands
        as_code = get_fncode_string(
            fncode) + " " + ra + ", " + rb + ", $" + str(imm12)
        return as_code
    elif isinstance(fncode, SBtypeFnCode):
        ra, rb, imm12 = operands
        as_code = get_fncode_string(
            fncode) + " " + ra + ", " + rb + ", $" + str(imm12)
        return as_code
    elif isinstance(fncode, UtypeFnCode):
        rd, imm20 = operands
        as_code = get_fncode_string(
            fncode) + " " + rd + ", $" + str(imm20)
        return as_code
    elif isinstance(fncode, UJtypeFnCode):
        rd, imm20 = operands
        as_code = get_fncode_string(
            fncode) + " " + rd + ", $" + str(imm20)
        return as_code
    else:
        raise ValueError(
            'FUCK! CHARLES EXPLODED!')
