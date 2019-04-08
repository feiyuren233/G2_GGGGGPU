#include "SigDriver.hpp"

#include "../obj_dir/VCore.h"
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

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    VCore *module = new VCore();
    auto driver = f(*module, OptSig(module->clk), OptSig(module->rstn));
    driver.setupTrace("name.vcd");
    driver.reset(10);

    // auto print = [&](){
    //     VL_PRINTF("a = %f, b = %f, c = %f, out = %f\n",
    //         module->a, module->b, module->c, module->out
    //     );
    // };

    std::vector<unsigned char> pixels(256 * 256 * 3);

    auto run = [&](int n) {
        for (int i = 0; i < n; i++)
        {
            //print();
            driver.tick();
            if (module->isStore)
            {
                for (int i = 0; i < 16; i++)
                {
                    // std::cout << "lol : " << std::endl;
                    if (module->storeMask & (1 << i))
                    {
                        // std::cout << "Storing : " << module->storeAddr[i] << std::endl;
                        unsigned base_addr = module->storeAddr[i] >> 2 * 3;
                        unsigned rgb = module->storeData[i];
                        unsigned char r = (rgb >> 16) & 0xff;
                        unsigned char g = (rgb >> 8) & 0xff;
                        unsigned char b = (rgb >> 0) & 0xff;
                        pixels[base_addr + 0] = r;
                        pixels[base_addr + 1] = g;
                        pixels[base_addr + 2] = b;
                    }
                }
            }
        }
    };

    module->wDataNext = 1;
    module->wDone = 1;

    for (int b = 0; b < 256; b += 1)
    {
        for (int i = 0; i < 16; i++)
        {
            module->inValid = 1;
            module->reqPC = 0;
            module->blockID = b;
            module->blockDim = 256;
            module->warpID = i;
            run(1);
        }
        run(1000);
        driver.reset(1);
    }

    // module->inValid = 1;
    // module->reqPC = 0;
    // module->blockID = 0;
    // module->blockDim = 10;
    // module->warpID = 0;
    // run(1);

    // module->inValid = 1;
    // module->reqPC = 0;
    // module->blockID = 0;
    // module->blockDim = 10;
    // module->warpID = 0;
    // run(1);

    //module->inValid = 0;
    run(2000);
    /* Add your code here */


    // for (int i = 0; i < 256; i++)
    // {
    //     for (int j = 0; j < 256; j++)
    //     {
    //         if (map[i][j] != 0)
    //         {
    //             pixels[i * 256 + j] = static_cast<unsigned char>(255);
    //         }
    //     }
    // }

    write_ppm("circle.ppm", pixels, 256, 256, 3);

    /* End your code here */

    module->final();
    delete module;
    module = NULL;
    return 0;
}