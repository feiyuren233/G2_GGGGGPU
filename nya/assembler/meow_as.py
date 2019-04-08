# python3

from assembler import run_assembler
from assembler_util import *
import sys
import argparse
try:
    import readline
except:
    pass  # readline not available


def shell_mode():
    print("WELCOME TO MEOW ASSEMBLER. Interactive Shell Mode. Enter <quit> to quit!")
    while True:
        line = input("> ")
        line = line.lower()
        if line.rstrip().lstrip() == "":
            continue
        if line == "quit":
            break
        print()
        try:
            run_assembler([line])
        except ValueError as e:
            print("Err: " + str(e))
            print()
            print("Help: You may want to go to <assembly_fncode_parser.py> and")
            print(
                "    : <assembly_operand_parser.py> to check for proper syntax supported.")

    print("BYE!")


def file_mode(assembly_file, machine_code_file, optimized_assembly_file):
    print("WELCOME TO MEOW ASSEMBLER. File Mode.")
    print("Assembly File:", assembly_file)
    print("Machine Code File:", machine_code_file)
    print("Optimized Assembly File:", optimized_assembly_file)
    print()

    with open(assembly_file) as ass_file:
        assembly_code_lines = ass_file.readlines()
        machine_codes, opt_assembly_codes = run_assembler(assembly_code_lines)

        if machine_code_file:
            with open(machine_code_file, "w") as ma_file:
                for word in machine_codes:
                    ma_file.write(str(get_bin(word, 32)))
                    ma_file.write("\n")
            print()
            print("Done! Go check your Machine Code File:", machine_code_file)
        else:
            print()
            print("Done!")

        if optimized_assembly_file:
            with open(optimized_assembly_file, "w") as opt_ass_file:
                for inst in opt_assembly_codes:
                    opt_ass_file.write(inst)
                    opt_ass_file.write("\n")
            print()
            print("Done! Go check your Optimized Assembly File:",
                  optimized_assembly_file)
        else:
            print()
            print("Done!")


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--source_assembly_file",
                    help="input source assembly file")
parser.add_argument("-o", "--output_binary_file",
                    help="output destination machine code binary file")
parser.add_argument("-t", "--optimized_assembly_file",
                    help="output destination optimized assembly file")
args = parser.parse_args()
if args.source_assembly_file:
    file_mode(args.source_assembly_file, args.output_binary_file,
              args.optimized_assembly_file)
else:
    shell_mode()
