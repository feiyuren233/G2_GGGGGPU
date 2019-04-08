#include "SigDriver.hpp"

#include "../obj_dir/VAXIMaster.h"

int main(int argc, char** argv){
    Verilated::commandArgs(argc, argv);
    VAXIMaster* module = new VAXIMaster();
    auto driver = f(*module, OptSig(module->M_AXI_ACLK), OptSig(module->M_AXI_ARESETN));
    driver.setupTrace("axi.vcd");
    driver.reset(1);

    auto print = [&](){
        // VL_PRINTF("a = %f, b = %f, c = %f, out = %f\n",
        //     module->a, module->b, module->c, module->out
        // );
    };

    auto run = [&](int n){
        for(int i = 0; i < n; i++){
            print();
            driver.tick();
        }
    };

    module->initWrite = 1;
    module->wData = 0xdeadbeef;
    module->writeLen = 1;
    module->writeAddress = 0x00ff00ff;
    module->wDataValid = 1;

    run(1);

    module->M_AXI_AWREADY = 1;
    run (1);
    module->M_AXI_AWREADY = 0;
    run (1);

    module->M_AXI_WREADY = 1;
    run(5);

    module->M_AXI_WREADY = 0;
    run (10);

    module->M_AXI_WREADY = 1;
    module->wDataValid = 0;
    
    run (10);
    module->wDataValid = 1;
    run(100);


    // Start a new write seq here

    module->initWrite = 0;
    run (1);
    module->initWrite = 1;
    run (1);
    module->M_AXI_AWREADY = 1;
    run (1);
    module->M_AXI_AWREADY = 0;
    run (1);

    module->M_AXI_WREADY = 1;
    run (100);




    /* Add your code here */





    /* End your code here */


    module->final();
    delete module; module = NULL;
    return 0;

}