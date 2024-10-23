#include <getopt.h>
#include <inttypes.h>
#include <sys/types.h>

#include "sail.h"
#include "riscv_platform_impl.h"


void show_addr_and_data(uint64_t addr, lbits data)
{
    fprintf(trace_log, "addr: %lu\n", addr);
    gmp_printf("data: %Zd \n", data.bits[0]);
    // fprintf(trace_log, "ovo");
}