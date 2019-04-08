int main(){
    int bdim = blkdim();
    int bidx = blkidx();
    int product = bdim * bidx;
    int tid = tidx();
    int globalID = product + tid;
    int width = 511;
    int x = width & globalID;
    int y = globalID >> 9;
    int storeAddr = globalID << 2;
    int deltax = x - 256;
    int deltay = y - 256;
    int dx2 = 0;
    int dy2 = 0;
    int distanceSquare = 0;
    int color = 255;
    if (deltax > 0){
        deltax = deltax;
    } else {
        deltax = 0 - deltax;
    }
    if (deltay > 0){
        deltay = deltay;
    } else {
        deltay = 0 - deltay;
    }
    dx2 = deltax * deltax;
    dy2 = deltay * deltay;
    distanceSquare = dx2 + dy2;
    color = color & (distanceSquare >> 4);
    color = (x >> 1) << 16;
    color = color | (y >> 1) << 8;
    color = color | ((deltax + deltay) >> 2);
    if (distanceSquare < (200 * 200)){
        distanceSquare = store(color, storeAddr, 0);
    }
    distanceSquare = exit();
}
