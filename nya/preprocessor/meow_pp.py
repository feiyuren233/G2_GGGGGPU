# python3

import os
import subprocess
import sys

if len(sys.argv) != 3:
    print("Err: Incorrect arguments!")
    print()
    print("python3 meow_pp.py [src_file_name] [dest_file_name]")
    print()
    print(":(")
    raise SystemExit

src_file_name = sys.argv[1]
dest_file_name = sys.argv[2]

print("WELCOME TO MEOW PREPROCESSOR.")
print("Source File:", src_file_name)
print("Destination File:", dest_file_name)
print()

p = subprocess.Popen(['gcc', '-E', '-P', src_file_name, '-o',
                      dest_file_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
gcc_out, gcc_err = p.communicate()
if gcc_err:
    print(gcc_out)
    print(gcc_err)

print()
print("Done! Go check your Destination File:", dest_file_name)
