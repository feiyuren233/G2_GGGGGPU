import re
from enum import Enum
from assembly_fncode_parser import *
from isa import *


# (rd, ra, rb)
def parse_rtype_operand(line):
    x = re.search(
        r"^[a-z_]+\s+([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*([rfi]\d+)\s*", line)
    if x:
        rd = x.group(1)
        ra = x.group(2)
        rb = x.group(3)
        print("rd = "+rd+" ra = " + ra + " rb = " + rb)

        return (rd, ra, rb)
    else:
        raise ValueError(
            'This line contains unsupported rtype operands: ' + line)


# (rd, ra, rb, rc)
def parse_rftype_operand(line):
    x = re.search(
        r"^[a-z_]+\s+([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*([rfi]\d+)\s*", line)
    if x:
        rd = x.group(1)
        ra = x.group(2)
        rb = x.group(3)
        rc = x.group(4)
        print("rd = " + rd + " ra = " + ra + " rb = " + rb + " rc = " + rc)

        return (rd, ra, rb, rc)
    else:
        raise ValueError(
            'This line contains unsupported rftype operands: ' + line)


# (rd, ra, imm12)
def parse_itype_operand(line, is_fp_imm):
    rstr = ""
    if is_fp_imm:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*\$([-+]?\d*\.?\d+([eE][-+]?\d+)?)\s*"
    else:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*\$([+-]?\d+)\s*"

    x = re.search(rstr, line)
    if x:
        rd = x.group(1)
        ra = x.group(2)
        imm12 = x.group(3)
        print("rd = "+rd+" ra = " + ra + " imm12 = " + imm12)

        if is_fp_imm:
            return (rd, ra, float(imm12))
        else:
            return (rd, ra, int(imm12))
    else:
        raise ValueError(
            'This line contains unsupported itype operands: ' + line)


# (ra, rb, imm12)
def parse_stype_operand(line, is_fp_imm):
    rstr = ""
    if is_fp_imm:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*\$([-+]?\d*\.?\d+([eE][-+]?\d+)?)\s*"
    else:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*\$([+-]?\d+)\s*"
    x = re.search(rstr, line)
    if x:
        ra = x.group(1)
        rb = x.group(2)
        imm12 = x.group(3)
        print("ra = "+ra+" rb = " + rb + " imm12 = " + imm12)

        if is_fp_imm:
            return (ra, rb, float(imm12))
        else:
            return (ra, rb, int(imm12))
    else:
        raise ValueError(
            'This line contains unsupported stype operands: ' + line)


# (ra, rb, imm12)
def parse_sbtype_operand(line, is_fp_imm):
    rstr = ""
    if is_fp_imm:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*\$([-+]?\d*\.?\d+([eE][-+]?\d+)?)\s*"
    else:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*([rfi]\d+)\s*,\s*\$([+-]?\d+)\s*"
    x = re.search(rstr, line)
    if x:
        ra = x.group(1)
        rb = x.group(2)
        imm12 = x.group(3)
        print("ra = "+ra+" rb = " + rb + " imm12 = " + imm12)

        if is_fp_imm:
            return (ra, rb, float(imm12))
        else:
            return (ra, rb, int(imm12))
    else:
        raise ValueError(
            'This line contains unsupported sbtype operands: ' + line)


# (rd, imm20)
def parse_utype_operand(line, is_fp_imm):
    rstr = ""
    if is_fp_imm:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*\$([-+]?\d*\.?\d+([eE][-+]?\d+)?)\s*"
    else:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*\$([+-]?\d+)\s*"
    x = re.search(rstr, line)
    if x:
        rd = x.group(1)
        imm20 = x.group(2)
        print("rd = "+rd+" imm20 = " + imm20)

        if is_fp_imm:
            return (rd, float(imm20))
        else:
            return (rd, int(imm20))
    else:
        raise ValueError(
            'This line contains unsupported utype operands: ' + line)


# (rd, imm20)
def parse_ujtype_operand(line, is_fp_imm):
    rstr = ""
    if is_fp_imm:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*\$([-+]?\d*\.?\d+([eE][-+]?\d+)?)\s*"
    else:
        rstr = r"^[a-z_]+\s+([rfi]\d+)\s*,\s*\$([+-]?\d+)\s*"
    x = re.search(rstr, line)
    if x:
        rd = x.group(1)
        imm20 = x.group(2)
        print("rd = "+rd+" imm20 = " + imm20)

        if is_fp_imm:
            return (rd, float(imm20))
        else:
            return (rd, int(imm20))
    else:
        raise ValueError(
            'This line contains unsupported ujtype operands: ' + line)


# (operands)
def parse_instruction_operands(fncode, line):
    if isinstance(fncode, RtypeFnCode):
        return parse_rtype_operand(line)
    elif isinstance(fncode, RFtypeFnCode):
        return parse_rftype_operand(line)
    elif isinstance(fncode, ItypeFnCode):
        return parse_itype_operand(line, is_imm_floating_point(fncode))
    elif isinstance(fncode, StypeFnCode):
        return parse_stype_operand(line, is_imm_floating_point(fncode))
    elif isinstance(fncode, SBtypeFnCode):
        return parse_sbtype_operand(line, is_imm_floating_point(fncode))
    elif isinstance(fncode, UtypeFnCode):
        return parse_utype_operand(line, is_imm_floating_point(fncode))
    elif isinstance(fncode, UJtypeFnCode):
        return parse_ujtype_operand(line, is_imm_floating_point(fncode))
    else:
        raise ValueError(
            'This line contains unsupported instruction: ' + line)
