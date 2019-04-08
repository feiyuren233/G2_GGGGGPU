# Meow Meow Meow GPU ASSEMBLY

# Instructions

# 2:    int bdim = blkdim();
blkdim      i1          ,  i0          ,  $233        
addi        i2          ,  i1          ,  $0          

# 3:    int bidx = blkidx();
blkidx      i3          ,  i2          ,  $233        
addi        i4          ,  i3          ,  $0          

# 4:    int product = bdim * bidx;
mul         i5          ,  i2          ,  i4          
addi        i6          ,  i5          ,  $0          

# 5:    int tid = tidx();
tidx        i7          ,  i6          ,  $233        
addi        i8          ,  i7          ,  $0          

# 6:    int globalID = product + tid;
add         i9          ,  i6          ,  i8          
addi        i10         ,  i9          ,  $0          

# 7:    int width = 511;
lli_i       i11         ,  $511        
addi        i12         ,  i11         ,  $0          

# 8:    int x = width & globalID;
and         i13         ,  i12         ,  i10         
addi        i14         ,  i13         ,  $0          

# 9:    int y = globalID >> 9;
ashr        i15         ,  i10         ,  $9          
addi        i16         ,  i15         ,  $0          

# 10:    int storeAddr = globalID << 2;
shl         i17         ,  i10         ,  $2          
addi        i18         ,  i17         ,  $0          

# 11:    int deltax = x - 256;
lli_i       i19         ,  $256        
sub         i20         ,  i14         ,  i19         
addi        i21         ,  i20         ,  $0          

# 12:    int deltay = y - 256;
lli_i       i22         ,  $256        
sub         i23         ,  i16         ,  i22         
addi        i24         ,  i23         ,  $0          

# 13:    int dx2 = 0;
lli_i       i25         ,  $0          
addi        i26         ,  i25         ,  $0          

# 14:    int dy2 = 0;
lli_i       i27         ,  $0          
addi        i28         ,  i27         ,  $0          

# 15:    int distanceSquare = 0;
lli_i       i29         ,  $0          
addi        i30         ,  i29         ,  $0          

# 16:    int color = 255;
lli_i       i31         ,  $255        
addi        i32         ,  i31         ,  $0          

# Evaluate if statement condition
# 17:    if (deltax > 0){
lli_i       i33         ,  $0          
cmpgt_i     i34         ,  i21         ,  i33         
if          i34         ,  i34         ,  $1234       

# Into if statement then block

# 18:        deltax = deltax;
addi        i21         ,  i21         ,  $0          

# Into if statement else block
else        i35         ,  i35         ,  $1234       

# 20:        deltax = 0 - deltax;
lli_i       i36         ,  $0          
sub         i37         ,  i36         ,  i21         
addi        i21         ,  i37         ,  $0          

# End if
endif       i35         ,  i35         ,  $1234       

# Evaluate if statement condition
# 22:    if (deltay > 0){
lli_i       i36         ,  $0          
cmpgt_i     i37         ,  i24         ,  i36         
if          i37         ,  i37         ,  $1234       

# Into if statement then block

# 23:        deltay = deltay;
addi        i24         ,  i24         ,  $0          

# Into if statement else block
else        i38         ,  i38         ,  $1234       

# 25:        deltay = 0 - deltay;
lli_i       i39         ,  $0          
sub         i40         ,  i39         ,  i24         
addi        i24         ,  i40         ,  $0          

# End if
endif       i38         ,  i38         ,  $1234       

# 27:    dx2 = deltax * deltax;
mul         i39         ,  i21         ,  i21         
addi        i26         ,  i39         ,  $0          

# 28:    dy2 = deltay * deltay;
mul         i40         ,  i24         ,  i24         
addi        i28         ,  i40         ,  $0          

# 29:    distanceSquare = dx2 + dy2;
add         i41         ,  i26         ,  i28         
addi        i30         ,  i41         ,  $0          

# 30:    color = color & (distanceSquare >> 4);
ashr        i42         ,  i30         ,  $4          
and         i43         ,  i32         ,  i42         
addi        i32         ,  i43         ,  $0          

# 31:    color = (x >> 1) << 16;
ashr        i44         ,  i14         ,  $1          
shl         i45         ,  i44         ,  $16         
addi        i32         ,  i45         ,  $0          

# 32:    color = color | (y >> 1) << 8;
ashr        i46         ,  i16         ,  $1          
shl         i47         ,  i46         ,  $8          
or          i48         ,  i32         ,  i47         
addi        i32         ,  i48         ,  $0          

# 33:    color = color | ((deltax + deltay) >> 2);
add         i49         ,  i21         ,  i24         
ashr        i50         ,  i49         ,  $2          
or          i51         ,  i32         ,  i50         
addi        i32         ,  i51         ,  $0          

# Evaluate if statement condition
# 34:    if (distanceSquare < (200 * 200)){
lli_i       i52         ,  $200        
lli_i       i53         ,  $200        
mul         i54         ,  i52         ,  i53         
cmplt_i     i55         ,  i30         ,  i54         
if          i55         ,  i55         ,  $1234       

# Into if statement then block

# 35:        distanceSquare = store(color, storeAddr, 0);
st_i        i18         ,  i32         ,  $0          
lli_i       i56         ,  $0          
addi        i30         ,  i56         ,  $0          

# End if
endif       i56         ,  i56         ,  $1234       

# 37:    distanceSquare = exit();
exit        i56         ,  i56         ,  $233        
lli_i       i57         ,  $0          
addi        i30         ,  i57         ,  $0          

