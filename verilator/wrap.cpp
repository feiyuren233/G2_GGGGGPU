#include "SigDriver.hpp"

#include "../obj_dir/VTop.h"
#include <stdlib.h>

#include <vector>
#include <fstream>
#include <cassert>
#include <iostream>

#define PIXEL_INDEX(x, y) ((y)*width + (x))

bool write_ppm(
    const std::string &filename,
    const std::vector<unsigned char> &data,
    const int width,
    const int height,
    const int num_channels)
{
    assert(
        (num_channels == 3 || num_channels == 1) &&
        ".ppm only supports RGB or grayscale images");

    std::fstream fs;
    fs.open(filename, std::fstream::out);

    /* Header section */
    // num channels
    fs << 'P' << ((num_channels == 3) ? 3 : 2) << std::endl;
    // width and height
    fs << width << ' ' << height << std::endl;
    // max value
    fs << 255 << std::endl;

    /* Image data section */
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int i = 0; i < num_channels; i++)
            {
                fs << static_cast<unsigned>(data[PIXEL_INDEX(x, y) * num_channels + i]);
                fs << ' ';
            }
            fs << "   ";
        }
        fs << std::endl;
    }
    fs.close();

    return true;
}

int main(int argc, char** argv){
    Verilated::commandArgs(argc, argv);
    VTop* module = new VTop();
    auto driver = f(*module, OptSig(module->clk), OptSig(module->rstn));
    driver.setupTrace("wrap.vcd");
    driver.reset(10);

    static size_t counter = 0;
    bool finished = false;

    auto print = [&](){
        // VL_PRINTF("a = %f, b = %f, c = %f, out = %f\n",
        //     module->a, module->b, module->c, module->out
        // );
    };

    int count = 0;
    bool started = 0;
    uint32_t baseAddr;

    std::vector<unsigned char> gt(512 * 512 * 3);
    std::vector<unsigned char> pixels(512 * 512 * 3);

    int state = 0;

    auto run = [&](int n){
        for(int i = 0; i < n; i++){
            driver.tick();
            if(module->goodJob && !finished){
                std::cout << "Job finished, cycles consumed = " << counter << std::endl;
                finished = true;
            }
            counter ++;

            if(module->M_AXI_AWVALID){
                module->M_AXI_AWREADY = 1;
                count = 0;
                started = 1;
                baseAddr = module->M_AXI_AWADDR >> 2;
                module->M_AXI_WREADY = 1;
                state = 1;
            }else if (state == 1){
                module->M_AXI_AWREADY = 1;
                state = 2;
                
            }else if (state == 2){
                module->M_AXI_AWREADY = 0;
                module->M_AXI_WREADY = 1;
                state = 0;
            }

            if(module->M_AXI_WVALID && count < 16){
                if(module->M_AXI_WSTRB){
                    pixels[baseAddr + count] = module->M_AXI_WDATA;
                }
                count++;
            }
            if(module->M_AXI_WLAST)
                module->M_AXI_WREADY = 1;

            if (module->isStore)
            {
                for (int i = 0; i < 16; i++)
                {
                    // std::cout << "lol : " << std::endl;
                    if (module->storeMask & (1 << i))
                    {
                        // std::cout << "Storing : " << (module->storeAddr[i] >> 2) << std::endl;
                        unsigned base_addr = (module->storeAddr[i] >> 2) * 3;
                        unsigned rgb = module->storeData[i];
                        unsigned char r = (rgb >> 16) & 0xff;
                        unsigned char g = (rgb >> 8) & 0xff;
                        unsigned char b = (rgb >> 0) & 0xff;
                        gt[base_addr + 0] = r;
                        gt[base_addr + 1] = g;
                        gt[base_addr + 2] = b;
                    }
                }
            }
        }
    };




    /* Add your code here */
    module->reqValid = 1;
    module->reqBlockDim = 512;
    module->reqWarpCount = 32;

    // module->M_AXI_AWREADY = 1;
    module->M_AXI_WREADY = 1;

    run(1000 * 1500);




    /* End your code here */

    // write_ppm("final.ppm", pixels, 256, 256, 3);
    write_ppm("gt.ppm", gt, 512, 512, 3);

    module->final();
    delete module; module = NULL;
    return 0;

}