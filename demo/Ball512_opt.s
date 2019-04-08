blkdim i0, i0, $233
addi i1, i0, $0
blkidx i0, i1, $233
addi i0, i0, $0
mul i0, i1, i0
addi i1, i0, $0
tidx i0, i1, $233
addi i0, i0, $0
add i0, i1, i0
addi i1, i0, $0
lli_i i0, $511
addi i0, i0, $0
and i0, i0, i1
addi i7, i0, $0
ashr i0, i1, $9
addi i2, i0, $0
shl i0, i1, $2
addi i1, i0, $0
lli_i i0, $256
sub i0, i7, i0
addi i4, i0, $0
lli_i i0, $256
sub i0, i2, i0
addi i5, i0, $0
lli_i i0, $0
addi i0, i0, $0
lli_i i0, $0
addi i6, i0, $0
lli_i i0, $0
addi i6, i0, $0
lli_i i0, $255
addi i3, i0, $0
lli_i i0, $0
cmpgt_i i0, i4, i0
if i0, i0, $1234
addi i4, i4, $0
else r0, r0, $1234
lli_i i0, $0
sub i0, i0, i4
addi i4, i0, $0
endif r0, r0, $1234
lli_i i0, $0
cmpgt_i i0, i5, i0
if i0, i0, $1234
addi i5, i5, $0
else r0, r0, $1234
lli_i i0, $0
sub i6, i0, i5
addi i5, i6, $0
endif r0, r0, $1234
mul i0, i4, i4
addi i0, i0, $0
mul i6, i5, i5
addi i6, i6, $0
add i0, i0, i6
addi i6, i0, $0
ashr i0, i6, $4
and i0, i3, i0
addi i3, i0, $0
ashr i0, i7, $1
shl i0, i0, $16
addi i3, i0, $0
ashr i0, i2, $1
shl i0, i0, $8
or i0, i3, i0
addi i3, i0, $0
add i0, i4, i5
ashr i0, i0, $2
or i0, i3, i0
addi i3, i0, $0
lli_i i2, $200
lli_i i0, $200
mul i0, i2, i0
cmplt_i i0, i6, i0
if i0, i0, $1234
st_i i1, i3, $0
lli_i i0, $0
addi i6, i0, $0
endif i0, i0, $1234
exit i0, i0, $233
lli_i i0, $0
addi i6, i0, $0
