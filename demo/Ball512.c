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

    /* placeholder for future variables */

    int distanceSquare = 0;

    /* constants */
    int color = 255;

    /* compute delta value of x */
    if (deltax > 0){
        deltax = deltax;
    } else {
        deltax = 0 - deltax;
    }

    /* compute delta value of y */
    if (deltay > 0){
        deltay = deltay;
    } else {
        deltay = 0 - deltay;
    }

    dx2 = deltax * deltax;
    dy2 = deltay * deltay;
    /* compute distance squared */
    distanceSquare = dx2 + dy2;
    color = color & (distanceSquare >> 4);

    color = (x >> 1) << 16;
    color = color | (y >> 1) << 8;
    /*color = color | ((distanceSquare >> 3) & 255);*/
    color = color | ((deltax + deltay) >> 2);

    /* Fragment shader initialization */
    if (distanceSquare < (200 * 200)){
        /* the current pixel is inside the circle */
        distanceSquare = store(color, storeAddr, 0);
    } 
        /* the current pixel is outside the circle */
        /* int temp = store(black, storeAddr, 0); */
    

    distanceSquare = exit();
}

