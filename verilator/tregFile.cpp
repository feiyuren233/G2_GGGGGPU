#include "../obj_dir/VRegFile.h"
#include <iostream>
#include <verilated.h>

//#define VM_TRACE 1

#if VM_TRACE
# include <verilated_vcd_c.h>
#endif

#include "SigDriver.hpp"

int main(int argc, char** argv){

    Verilated::commandArgs(argc, argv);
    VRegFile* reg = new VRegFile();
    auto driver = f(*reg, OptSig(reg->clk), OptSig(reg->clk), reg->ar0, reg->ar1, reg->aw);
    driver.setupTrace("RegFile.vcd");
    //driver.reset(1);
    reg->ar0 = 1;
    reg->ar1 = 4;
    reg->aw = 1;
    driver.print();
    driver.tick();
    driver.print();
    std::cout << std::get<0>(driver.m_sigs) << std::endl;
    

    reg->final();


    delete reg; reg = NULL;
}
