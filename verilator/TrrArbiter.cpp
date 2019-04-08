#include "../obj_dir/VrrArbiter.h"
#include <iostream>
#include <verilated.h>

//#define VM_TRACE 1

#if VM_TRACE
# include <verilated_vcd_c.h>
#endif

#include "SigDriver.hpp"

int main(int argc, char** argv){

    Verilated::commandArgs(argc, argv);
    VrrArbiter* arb = new VrrArbiter();

    vluint64_t main_time = 0;
#if VM_TRACE
    // If verilator was invoked with --trace argument,
    // and if at run time passed the +trace argument, turn on tracing
    VerilatedVcdC* tfp = NULL;
    const char* flag = Verilated::commandArgsPlusMatch("trace");
    if (flag && 0==strcmp(flag, "+trace")) {
        Verilated::traceEverOn(true);  // Verilator must compute traced signals
        VL_PRINTF("Enabling waves into logs/vlt_dump.vcd...\n");
        tfp = new VerilatedVcdC();
        arb->trace(tfp, 99);  // Trace 99 levels of hierarchy
        Verilated::mkdir("logs");
        tfp->open("logs/vlt_dump.vcd");  // Open the dump file
    }
#endif

    auto cycle = [&](int num = 1) -> void{
        for(int i = 0; i < num; i++){
            arb->clk = 0;
            arb->eval();
            main_time ++;
            #if VM_TRACE
                // Dump trace data for this cycle
                if (tfp) tfp->dump (main_time);
            #endif
            arb->clk = 1;
            arb->eval();
            main_time ++;
            #if VM_TRACE
        // Dump trace data for this cycle
        if (tfp) tfp->dump (main_time);
#endif
        }
    };

    arb -> reset = 0;
    cycle();
    arb -> reset = 1;
    cycle();

    auto test = [&](uint32_t val) -> void{
        arb->req = val;
        for(int i = 0; i < 16; i++){
            cycle();
            VL_PRINTF("req = %x --- grant = %x\n", val, arb->grant);
        }
    };

    test(0b0011001100110011);
    auto driver = f(*arb, OptSig(arb->clk), OptSig(arb->reset), arb->req);
    std::cout << std::get<0>(driver.m_sigs) << std::endl;
    test(0b1000000000000001);
    std::cout << std::get<0>(driver.m_sigs) << std::endl;
    test(0b0000000000000001);
    std::cout << std::get<0>(driver.m_sigs) << std::endl;
    test(0b1000000000000000);
    test(0b0100000100100001);
    std::cout << std::get<0>(driver.m_sigs) << std::endl;

    arb -> req = 0b1;
    cycle();
    arb -> req = 0;
    cycle();
    cycle();


    //#include "SigDriver.hpp"
    

    


    arb->final();

    #if VM_TRACE
    if (tfp) { tfp->close(); tfp = NULL; }
#endif

    delete arb; arb = NULL;
}
