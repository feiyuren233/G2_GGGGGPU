#include "../obj_dir/VIntResvStation.h"
#include "SigDriver.hpp"


int main(int argc, char** argv){
    Verilated::commandArgs(argc, argv);
    VIntResvStation* station = new VIntResvStation();
    auto driver = f(*station, OptSig(station->clk), OptSig(station->reset));
    driver.setupTrace("intResvStation.vcd");

};