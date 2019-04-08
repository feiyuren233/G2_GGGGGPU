# python3

import os
import subprocess
import sys
import shutil


preprocessor_exe = "./preprocessor/meow_pp.py"
frontend_dir = "./frontend/"
frontend_exe = frontend_dir + "compiler467"
assembler_exe = "./assembler/meow_as.py"
temp_dir = "./__TEMP__/"

dest_file_name = ""
if len(sys.argv) == 2:
    dest_file_name = None
elif len(sys.argv) == 3:
    dest_file_name = sys.argv[2]
else:
    print("Err: Incorrect arguments!")
    print()
    print("python3 meow.py [src_file_name] <dest_file_name>")
    print()
    print(":(")
    raise SystemExit

src_file_name = sys.argv[1]


print("WELCOME TO MEOW COMPILER TOOLCHAIN.")
print("Source File:", src_file_name)
print("Destination File:", dest_file_name)
print()


# Remove previous temp files
if os.path.exists(temp_dir):
    shutil.rmtree(temp_dir)

# Create temp folder
os.makedirs(temp_dir)

# Create temp files
src_file_core = os.path.splitext(os.path.basename(src_file_name))[0]
pp_file = temp_dir + src_file_core + "_pp.c"
fe_file = temp_dir + src_file_core + "_fe.s"
opt_as_file = temp_dir + src_file_core + "_opt.s"


# PREPROCESSOR
print("Preprocessor")
p = subprocess.Popen(['python3', preprocessor_exe, src_file_name,
                      pp_file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
pp_out, pp_err = p.communicate()
if pp_err:
    print(pp_out)
    print(pp_err)
    print()
    raise Exception("preprocessor failed")
print("    Done! Preprocessed File at", pp_file)


# FRONTEND
print("Frontend")

# Rebuild - make clean
print("    Rebuilding: ",  end="")
p = subprocess.Popen(['make', 'clean'], stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE, cwd=frontend_dir)
make_clean_out, make_clean_err = p.communicate()
if make_clean_err:
    print(make_clean_out)
    print(make_clean_err)
    print()
    raise Exception("make clean failed")
else:
    print("..clean", end="")
# Rebuild - make
p = subprocess.Popen(['make'], stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE, cwd=frontend_dir)
make_out, make_err = p.communicate()
# print(make_out)
# print(make_err)
print("..make")

# Compile
print("    Compiling:")
p = subprocess.Popen([frontend_exe, '-Dx', '-U', fe_file,
                      pp_file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
fe_out, fe_err = p.communicate()
if fe_err:
    sys.stdout.write(fe_err.decode('utf-8'))
if fe_out:
    sys.stdout.write(fe_out.decode('utf-8'))
    if fe_out.find(b"Failed to compile") != -1:
        print()
        raise Exception("frontend failed")
print("    Done! Processed File at", fe_file)


# ASSEMBLER
print("Assembler")
assembler_args = ['python3', assembler_exe, "-s", fe_file, "-t", opt_as_file]
if dest_file_name:
    assembler_args.append("-o")
    assembler_args.append(dest_file_name)
p = subprocess.Popen(assembler_args, stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE)
as_out, as_err = p.communicate()
if as_err:
    sys.stdout.write(as_out.decode('utf-8'))
    sys.stdout.write(as_err.decode('utf-8'))
    print()
    raise Exception("assembler failed")
print("    Done! Assembled File at", dest_file_name)


# ALL DONE
print()
print("Successful!")
print("Source File:", src_file_name)
print("Destination File:", dest_file_name)
