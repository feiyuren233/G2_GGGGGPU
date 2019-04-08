cmake_minimum_required(VERSION 3.0.0)

project (meow)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES hardware/*.sv hardware/*.v hardware/*.svh hardware/*.vh testbench/*.sv testbench/*.v testbench/*.svh testbench/*.vh)

add_custom_target(loc
  COMMAND wc -l nya/*/*.py nya/*/*.cpp nya/*/*.h nya/*/tests/*.py hardware/*.* verilator/*.*
)

add_custom_target(refresh 
  COMMAND rm -f CMakeCache.txt
  COMMAND rm -f cmake_install.cmake
  COMMAND rm -f ./obj_dir/*.d ./obj_dir/*o
  COMMAND rm -rf ./obj_dir
  COMMAND find  -type f -exec touch {} +

  WORKING_DIRECTORY "./"
  COMMENT "Refreshing"
)

SET(DEP_MODULE)
SET(MODULE_NAMES)

function(ADD_MODULE top sim)
    add_custom_target(veri-${top}
      COMMAND verilator --top-module ${top} -Ihardware --cc ${SOURCES}
      COMMAND verilator --top-module ${top} -CFLAGS -O1 -CFLAGS -std=c++11  -Ihardware --trace -cc -exe ${SOURCES} verilator/${sim}.cpp
      WORKING_DIRECTORY "./"
      COMMENT "Verilating ${top}"
    )

    add_custom_target(build-${top} 
      DEPENDS veri-${top}
      COMMAND make -C obj_dir -f V${top}.mk V${top}

      WORKING_DIRECTORY "./"
      COMMENT "Building simulation model ${top}"
    ) 

    add_custom_target(${top}
      DEPENDS build-${top}
      COMMAND ./obj_dir/V${top} +trace

      WORKING_DIRECTORY "./"
      COMMENT "Running simulation model ${top}"
    )

    SET(DEP_MODULE veri-${top} ${DEP_MODULE} PARENT_SCOPE)
    SET(MODULE_NAMES ${top} ${MODULE_NAMES} PARENT_SCOPE)

endfunction()
    
add_custom_target(lint 

  COMMAND verilator --lint-only -Wall -Ihardware ${SOURCES} || (exit 0)

  WORKING_DIRECTORY "./"
  COMMENT "Linting verilog code"
)

add_module(rrArbiter TrrArbiter)
add_module(count_lead_zero TrrArbiter)
add_module(FPRegFile tregFile)
add_module(IntResvStation tIntResvStation)
add_module(dram tRam)
add_module(FMA_sim tb_fma)
add_module(rrArbiterHybrid TrrArbiterHybrid)
add_module(Core top)
add_module(AXIMaster axiburst)
add_module(popcnt popcnt)
add_module(Top wrap)

