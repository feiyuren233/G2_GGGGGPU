Info: Optimization for declaration of const-qualified symbol 'nishuile' of type 'const bool' successful at Line 3:5 to Line 3:32.
# Meow Meow Meow GPU ASSEMBLY

# Instructions

# 3:    const bool nishuile = true;
lli_i       i0          ,  $1          
addi        i1          ,  i0          ,  $0          

# Evaluate if statement condition
# 6:    if (nishuile)
if          i1          ,  i1          ,  $1234       

# Into if statement then block

# 8:        a = 2;
lli_i       i4          ,  $2          
addi        i2          ,  i4          ,  $0          

# End if
endif       i4          ,  i4          ,  $1234       

# 10:    c = a;
addi        i3          ,  i2          ,  $0

