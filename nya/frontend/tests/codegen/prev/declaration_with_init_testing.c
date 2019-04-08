Info: Optimization for declaration of const-qualified symbol 'b' of type 'const float' successful at Line 4:5 to Line 4:42.
# Meow Meow Meow GPU ASSEMBLY

# Instructions

# 3:    int a = 2 + 6;
lli_i       i0          ,  $2          
lli_i       i1          ,  $6          
add         i2          ,  i0          ,  i1          
addi        i3          ,  i2          ,  $0          

# 4:    const float b = 3.1415926535 + 1.000;
lli_f       f0          ,  $4.141593   
addi        f1          ,  f0          ,  $0          

# 5:    bool c = true && false;
lli_i       i4          ,  $1          
lli_i       i5          ,  $0          
and         i6          ,  i4          ,  i5          
addi        i7          ,  i6          ,  $0

