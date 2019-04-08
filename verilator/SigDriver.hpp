#ifndef SIG_DRIVER
#define SIG_DRIVER

#include <verilated.h>

#include <string>
#include <iostream>
#include <ostream>


#if VM_TRACE
# include <verilated_vcd_c.h>
#endif

class OptSig{
    public:
        OptSig(CData& signal) : m_signal(&signal){}
        OptSig() : m_signal(NULL){}
        static OptSig NullSig() {return OptSig();}

        CData* m_signal;
};

template<std::size_t> struct int_{};

template <class Tuple, size_t Pos>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, int_<Pos> ) {
  out << std::get< std::tuple_size<Tuple>::value-Pos >(t) << ',';
  return print_tuple(out, t, int_<Pos-1>());
}

template <class Tuple>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, int_<1> ) {
  return out << std::get<std::tuple_size<Tuple>::value-1>(t);
}

template <class... Args>
std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& t) {
  out << '('; 
  print_tuple(out, t, int_<sizeof...(Args)>()); 
  return out << ')';
}

template<typename module_t, typename... Args>
class SigDriver{
public:
    SigDriver(module_t& module, OptSig& clk, OptSig& reset, Args&... args) : m_moudle(module), m_sigs(args...), m_clk(clk.m_signal), m_reset(reset.m_signal){
        tfp = NULL;
        main_time = 0;
    }

    ~SigDriver(){
        #if VM_TRACE
        if (tfp) { tfp->close(); tfp = NULL; }
        #endif
    }

public:
    void setInput(const Args&... args){
        m_sigs = std::make_tuple(args...);
    }

    vluint64_t getTime(){
        return main_time / 2;
    }

    void low(){

    }

    void tick(int cycles = 1){
        m_moudle.eval();

        if(this->m_clk)
            *(this->m_clk) = 0;
        m_moudle.eval();
        trace();
        main_time ++;

        if(this->m_clk)
            *(this->m_clk) = 1;
        m_moudle.eval();
        trace();
        main_time ++;
        
    }

    void reset(int cycles, bool activeLow = true){
        if (!this->m_reset) return;
        for(int i = 0; i < cycles; i++){
            *(this->m_reset) = !activeLow;
            this->tick();
        }
        *(this->m_reset) = activeLow;
            this->tick();
    }

    void setupTrace(std::string path){
        #if VM_TRACE
        const char* flag = Verilated::commandArgsPlusMatch("trace");
        if (flag && 0==strcmp(flag, "+trace")) {
            Verilated::traceEverOn(true);  // Verilator must compute traced signals
            //VL_PRINTF("Enabling waves into trace/" + path + "\n");
            tfp = new VerilatedVcdC();
            m_moudle.trace(tfp, 99);  // Trace 99 levels of hierarchy
            Verilated::mkdir("trace");
            std::string str = std::string("trace/") + path;
            tfp->open(str.c_str());  // Open the dump file
        }
        #endif
    }

    void print(){
        std::cout << this->m_sigs << std::endl;
    }

public:
    std::tuple<Args&...> m_sigs;

private:
    module_t& m_moudle;
    CData* m_clk, *m_reset;

    vluint64_t main_time;

    VerilatedVcdC* tfp;

private:
    void trace(){
        if (tfp) tfp->dump (main_time);
    }
};

template<class module_t, class ...Ts> 
SigDriver<module_t, Ts&...> f(module_t& module, OptSig&& clk, OptSig&& reset, Ts&... args) {
    SigDriver<module_t, Ts&...> driver(module, clk, reset, args...);
    return driver;
}

#endif