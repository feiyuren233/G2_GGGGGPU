# Meow Meow Meow GPU ASSEMBLY

# Instructions

# 4:    a = blkdim();
blkdim      i1          ,  i0          ,  $233        
addi        i0          ,  i1          ,  $0          

# 5:    a = blkidx();
blkidx      i2          ,  i1          ,  $233        
addi        i0          ,  i2          ,  $0          

# 6:    a = tidx();
tidx        i3          ,  i2          ,  $233        
addi        i0          ,  i3          ,  $0          

# 7:    a = exit();
exit        i3          ,  i3          ,  $233        
lli_i       i4          ,  $0          
addi        i0          ,  i4          ,  $0          

# 8:    a = store(123, 128, 16);
lli_i       i5          ,  $123        
lli_i       i6          ,  $128        
st_i        i6          ,  i5          ,  $16         
lli_i       i7          ,  $0          
addi        i0          ,  i7          ,  $0          

# 9:    a = store(3.1415926, 256, 32);
lli_f       f0          ,  $3.141593   
lli_i       i8          ,  $256        
st_f        i8          ,  f0          ,  $32         
lli_i       i9          ,  $0          
addi        i0          ,  i9          ,  $0

