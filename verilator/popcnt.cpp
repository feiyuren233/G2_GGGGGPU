#include "SigDriver.hpp"

#include "../obj_dir/Vpopcnt.h"

int main(int argc, char** argv){
    Verilated::commandArgs(argc, argv);
    Vpopcnt* module = new Vpopcnt();
    auto driver = f(*module, OptSig::NullSig(), OptSig::NullSig());
    driver.setupTrace("popcnt.vcd");
    // driver.reset(1);

    auto print = [&](){
        VL_PRINTF("in = %x, out = %d\n",
            module->in, module->pcnt
        );
    };

    auto run = [&](int n){
        for(int i = 0; i < n; i++){
            driver.tick();
            print();
        }
    };

    /* Add your code here */
    module->in = 0b10101;
    run(1);
    module->in = 0b11111;
    run(1);





    /* End your code here */


    module->final();
    delete module; module = NULL;
    return 0;

}