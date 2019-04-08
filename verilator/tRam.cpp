#include "../obj_dir/Vdram.h"
#include <iostream>
#include <verilated.h>

//#define VM_TRACE 1

#if VM_TRACE
# include <verilated_vcd_c.h>
#endif

#include "SigDriver.hpp"

#include <unordered_map>

#define WIDTH (14)
#define BURST_MAX_BITS (8)

int main(int argc, char** argv){
    Verilated::commandArgs(argc, argv);
    Vdram* ram = new Vdram();
    auto driver = f(*ram, OptSig(ram->clk), OptSig(ram->rst));
    driver.setupTrace("ram.vcd");
    driver.reset(1);
    

    auto print = [&](){
        VL_PRINTF("rAddr = %x, arValid = %x, arReady = %x, arLen = %d, rValid = %x, rReady = %x, rData = %x\n",
            ram->rAddr, ram->arValid, ram->arReady, ram->arLen, ram->rValid, ram->rReady, ram->rData
        );
    };

    auto runNcycles = [&](int n){
        for(int i = 0; i < n; i++){
            print();
            driver.tick();
            
        }
    };

    /* TEST MISSING */
    // Read & Write collision?


    print();
    runNcycles(1);
    ram->rAddr = 4;
    ram->arValid = 1;
    ram->arLen = 4;
    ram->rReady = 1;
    
    runNcycles(1);
    ram->arValid = false;

    runNcycles(2);
    ram->rReady = 0;
    runNcycles(1);
    ram->rReady = 1;
    runNcycles(6);
    ram->wAddr = 0;
    ram->awValid = 1;
    ram->awLen = 3;
    ram->wData = 0xafafafaf;
    ram->wStrb = 0b1001;
    ram->wValid = 1;

    runNcycles(6);
    ram->rAddr = 0;
    ram->arLen = 5;
    ram->arValid = true;
    runNcycles(8);





    ram->final();
    delete ram; ram = NULL;
    return 0;
}