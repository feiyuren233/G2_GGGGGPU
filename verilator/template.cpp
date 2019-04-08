#include "SigDriver.hpp"

#include "../obj_dir/<header>.h"

int main(int argc, char** argv){
    Verilated::commandArgs(argc, argv);
    <Module>* module = new <Module>();
    auto driver = f(*module, OptSig(module->clk), OptSig(module->rst));
    driver.setupTrace("name.vcd");
    driver.reset(1);

    auto print = [&](){
        VL_PRINTF("a = %f, b = %f, c = %f, out = %f\n",
            module->a, module->b, module->c, module->out
        );
    };

    auto run = [&](int n){
        for(int i = 0; i < n; i++){
            print();
            driver.tick();
        }
    };

    /* Add your code here */





    /* End your code here */


    module->final();
    delete module; module = NULL;
    return 0;

}