#include <iostream>
#include <string>
#include "riscv_cpp_test.h"
#include "riscv_cache.h"

class S {
    std::string s = "Hello CPP !";
public:
    const std::string& get() {return s;}
    
};

static cache_sim_t dc(128, 4, 64, "L1 dcache"), 
                   ic(256, 2, 64, "L1 icache"),
                   l2(2048, 16, 64, "L2 cache");


extern "C" {

unit cache_init() {
    ic.set_miss_handler(&l2);
    dc.set_miss_handler(&l2);
    return UNIT;
}

unit dc_read(uint64_t addr) {
    dc.access(addr, 64, false);
    return UNIT;
}

unit ic_read(uint64_t addr) {
    ic.access(addr, 64, false);
    return UNIT;
}

unit dc_write(uint64_t addr) {
    dc.access(addr, 64, true);
    return UNIT;
}

unit print_dc_stats() {
    dc.print_stats();
    return UNIT;
}

unit print_ic_stats() {
    ic.print_stats();
    return UNIT;
}

unit print_l2_stats() {
    l2.print_stats();
    return UNIT;
}

}