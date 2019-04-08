import re
from isa import *


def parse_rtype_fncode(line):
    for fncode in RtypeFnCode:
        if re.search(r"^" + get_fncode_string(fncode) + r"\s+", line):
            return fncode
    return None


def parse_rftype_fncode(line):
    for fncode in RFtypeFnCode:
        if re.search(r"^" + get_fncode_string(fncode) + r"\s+", line):
            return fncode
    return None


def parse_itype_fncode(line):
    for fncode in ItypeFnCode:
        if re.search(r"^" + get_fncode_string(fncode) + r"\s+", line):
            return fncode
    return None


def parse_stype_fncode(line):
    for fncode in StypeFnCode:
        if re.search(r"^" + get_fncode_string(fncode) + r"\s+", line):
            return fncode
    return None


def parse_sbtype_fncode(line):
    for fncode in SBtypeFnCode:
        if re.search(r"^" + get_fncode_string(fncode) + r"\s+", line):
            return fncode
    return None


def parse_utype_fncode(line):
    for fncode in UtypeFnCode:
        if re.search(r"^" + get_fncode_string(fncode) + r"\s+", line):
            return fncode
    return None


def parse_ujtype_fncode(line):
    for fncode in UJtypeFnCode:
        if re.search(r"^" + get_fncode_string(fncode) + r"\s+", line):
            return fncode
    return None


def parse_instruction_fncode(line):
    fncode = parse_rtype_fncode(line)
    if fncode:
        return fncode

    else:
        fncode = parse_rftype_fncode(line)
        if fncode:
            return fncode

        else:
            fncode = parse_itype_fncode(line)
            if fncode:
                return fncode

            else:
                fncode = parse_stype_fncode(line)
                if fncode:
                    return fncode

                else:
                    fncode = parse_sbtype_fncode(line)
                    if fncode:
                        return fncode

                    else:
                        fncode = parse_utype_fncode(line)
                        if fncode:
                            return fncode

                        else:
                            fncode = parse_ujtype_fncode(line)
                            if fncode:
                                return fncode

                            else:
                                raise ValueError(
                                    'This line contains unsupported instruction: ' + line)
