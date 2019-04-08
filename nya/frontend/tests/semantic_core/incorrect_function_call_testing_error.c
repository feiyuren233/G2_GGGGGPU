int main()
{
    int inta;
    float floatb;
    floatb = blkdim();
    inta = blkidx(123);
    inta = tidx();
    floatb = exit();
    inta = store(123, 128.1, 16);
    inta = store(123, 128, inta);
    inta = store(3.1415926, 256, 32);
}