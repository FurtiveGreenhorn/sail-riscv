#include <iostream>
#include <string>
#include "riscv_cpp_test.h"
#include "riscv_cache.h"

class S {
    std::string s = "Hello CPP !";
public:
    const std::string& get() {return s;}
    
};

enum write_kind{
  Write_plain,
  Write_RISCV_release,
  Write_RISCV_strong_release,
  Write_RISCV_conditional,
  Write_RISCV_conditional_release,
  Write_RISCV_conditional_strong_release,
};

static cache_sim_t dc(128, 4, 64, "L1 dcache"), 
                   ic(256, 2, 64, "L1 icache"),
                   l2(2048, 16, 64, "L2 cache");


unit enum_test(const int wk) {
    switch (wk) {
        case Write_plain: std::cout << "Write_plain" << std::endl; break;
        case Write_RISCV_release: std::cout << "Write_RISCV_release" << std::endl; break;
        case Write_RISCV_strong_release: std::cout << "Write_RISCV_strong_release" << std::endl; break;
        case Write_RISCV_conditional: std::cout << "Write_RISCV_conditional" << std::endl; break;
        case Write_RISCV_conditional_release: std::cout << "Write_RISCV_conditional_release" << std::endl; break;
        case Write_RISCV_conditional_strong_release: std::cout << "Write_RISCV_conditional_strong_release" << std::endl; break;
    }
    return UNIT;
}

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