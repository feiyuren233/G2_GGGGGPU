# Meow Meow Meow GPU ASSEMBLY

# Instructions

# 3:    int inta = 3;
lli_i       i0          ,  $3          
addi        i1          ,  i0          ,  $0          

# 4:    inta = inta + 2;
lli_i       i2          ,  $2          
add         i3          ,  i1          ,  i2          
addi        i1          ,  i3          ,  $0          

# Evaluate if statement condition
# 5:    if (inta > 2)
lli_i       i4          ,  $2          
cmpgt_i     i5          ,  i1          ,  i4          
if          i5          ,  i5          ,  $1234       

# Into if statement then block

# 7:        int intb = 3;
lli_i       i6          ,  $3          
addi        i7          ,  i6          ,  $0          

# Evaluate if statement condition
# 8:        if (inta > intb)
cmpgt_i     i8          ,  i1          ,  i7          
if          i8          ,  i8          ,  $1234       

# Into if statement then block

# 10:            intb = 5;
lli_i       i9          ,  $5          
addi        i7          ,  i9          ,  $0          

# Into if statement else block
else        i9          ,  i9          ,  $1234       

# 14:            intb = 6;
lli_i       i10         ,  $6          
addi        i7          ,  i10         ,  $0          

# End if
endif       i9          ,  i9          ,  $1234       

# Into if statement else block
else        i6          ,  i6          ,  $1234       

# 19:        int intb = 4;
lli_i       i7          ,  $4          
addi        i8          ,  i7          ,  $0          

# End if
endif       i6          ,  i6          ,  $1234

