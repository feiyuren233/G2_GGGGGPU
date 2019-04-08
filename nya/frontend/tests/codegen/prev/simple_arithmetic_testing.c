Info: Optimization for declaration of const-qualified symbol 'pi' of type 'const float' successful at Line 3:5 to Line 3:36.
# Meow Meow Meow GPU ASSEMBLY

# Instructions

# 3:    const float pi = 3.14159 * 2.0;
lli_f       f0          ,  $6.283180   
addi        f1          ,  f0          ,  $0          

# 4:    float two_pi = 2.0 * pi;
lli_f       f2          ,  $2.000000   
mul_f       f3          ,  f2          ,  f1          
addi        f4          ,  f3          ,  $0          

# 6:        float pi = 1.0;
lli_f       f5          ,  $1.000000   
addi        f6          ,  f5          ,  $0

