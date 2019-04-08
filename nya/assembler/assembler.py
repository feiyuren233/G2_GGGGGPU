from assembly_fncode_parser import *
from assembly_operand_parser import *
from codegen import *
from assembler_util import *

from optimizer import *


# (machine_code, assembly_string)
def run_assembler(lines):
    assembly_code = []
    # printable assembly code
    assembly_string = []
    machine_code = []
    for line in lines:
        # Remove comments starting with #
        line = preprocess_line(line)

        # Remove trailing spaces
        line = line.rstrip().lstrip()
        if line == "":
            continue

        print(line)

        # Parse assembly instruction
        instruction = parse_assembly_instruction(line)
        assembly_code.append(instruction)
        assembly_string.append(generate_assembly_code(instruction))

        # Code generation
        inst_word = generate_machine_code(instruction)
        machine_code.append(inst_word)

        print()

    # Pass to optimizer
    r = optimizer(assembly_code)
    if r:
        print()
        print("Assembly code optimized!")
        print()
        # Regenerate machine code
        machine_code = []
        assembly_string = []
        for inst in r:
            assembly_str = generate_assembly_code(inst)
            print(assembly_str)
            assembly_string.append(assembly_str)
            inst_word = generate_machine_code(inst)
            machine_code.append(inst_word)
            print()

    # Display MACHINE CODE in BIN
    print("MACHINE CODE BIN")
    for word in machine_code:
        print(str(get_bin(word, 32)))

    # Display MACHINE CODE in DEC
    print()
    print("MACHINE CODE DEC")
    for word in machine_code:
        print(word)

    return (machine_code, assembly_string)


def preprocess_line(line):
    line, _, _ = line.partition('#')
    return line


# (fncode, (operands))
def parse_assembly_instruction(line):
    fncode = parse_instruction_fncode(line)
    print(fncode)

    return (fncode,) + (parse_instruction_operands(fncode, line),)
