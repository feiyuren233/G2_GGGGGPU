#include "SigDriver.hpp"

#include "../obj_dir/VFMA_sim.h"

int main(int argc, char** argv){
    Verilated::commandArgs(argc, argv);
    VFMA_sim* module = new VFMA_sim();
    auto driver = f(*module, OptSig(module->clk), OptSig::NullSig());
    driver.setupTrace("fma.vcd");
    //driver.reset(1);

    auto print = [&](){
        VL_PRINTF("[time = %d]  a = %f, b = %f, c = %f, out = %f, clken = %x\n",
            driver.getTime(), module->a, module->b, module->c, module->out, module->clken
        );
    };

    auto run = [&](int n){
        for(int i = 0; i < n; i++){
            print();
            driver.tick();
        }
    };
    /* Add your code here */
    module->clken = 1;
    module->a = 1.0;
    module->b = 2.0;
    module->c = 3.0;
    module->mode = 1;
    run(30);

    module->clken = 0;
    module->a = 1.0;
    module->b = 2.0;
    module->c = 3.0;
    module->mode = 0;
    run(10);
    module->clken = 1;
    run(30);


    /* End your code here */


    module->final();
    delete module; module = NULL;
    return 0;

}