# meowGPU
meow

# Introduction
See documentation [here](intro/README.md):
"intro/README.md"

# To check line of code
    make loc

# Prerequisites
    -cmake >= 3.0.0
    -Verilator >= 4.0.0
    
# Optional
    -GTKWave
    
# Generate
    cmake .

# Add new module for testing
### Add to CMakeList.txt
    add_module(<module name> <simulation source>)

# To run automatic testing pipeline
    make <module name>

# Lint 
### Prefer no warnings & errors
    make lint
    

    
