#pragma once
#include <verilated.h>
#if VM_TRACE
    #include <verilated_vcd_c.h>
#endif

#include <tuple>


template<typename Module>
class TestbenchBase{
    template<typename* interface... >
    class interface_type{
        
    }

    public:
        TestbenchBase(Module* module);
        ~TestbenchBase();

    

    public:
        virtual void reset(int cycles);

    public:
        virtual void tick();

    protected:
        Module* m_Module;
        vluint64_t main_time;
        std::tuple
};