
#pragma once
#include "sail.h"

#ifdef __cplusplus
extern "C" {
#endif

unit cache_init();
unit dc_read(uint64_t addr);
unit ic_read(uint64_t addr);
unit dc_write(uint64_t addr);
unit print_dc_stats();
unit print_ic_stats();
unit print_l2_stats();
#ifdef __cplusplus
}
#endif