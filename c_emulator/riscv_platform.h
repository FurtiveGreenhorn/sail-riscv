#pragma once
#include "sail.h"

bool sys_enable_rvc(unit);
bool sys_enable_fdext(unit);
bool sys_enable_svinval(unit);
bool sys_enable_zcb(unit);
bool sys_enable_zfinx(unit);
bool sys_enable_writable_misa(unit);
bool sys_enable_writable_fiom(unit);
bool sys_enable_vext(unit);
bool sys_enable_bext(unit);
bool sys_enable_zicbom(unit);
bool sys_enable_zicboz(unit);

uint64_t sys_pmp_count(unit);
uint64_t sys_pmp_grain(unit);

bool sys_vext_vl_use_ceil(unit);
uint64_t sys_vector_vlen_exp(unit);
uint64_t sys_vector_elen_exp(unit);

bool plat_enable_dirty_update(unit);
bool plat_enable_misaligned_access(unit);
bool plat_mtval_has_illegal_inst_bits(unit);
mach_bits sys_writable_hpm_counters(unit u);

mach_bits plat_ram_base(unit);
mach_bits plat_ram_size(unit);
bool within_phys_mem(mach_bits, sail_int);

mach_bits plat_rom_base(unit);
mach_bits plat_rom_size(unit);

mach_bits plat_cache_block_size_exp(unit);

// Provides entropy for the scalar cryptography extension.
mach_bits plat_get_16_random_bits(unit);

mach_bits plat_clint_base(unit);
mach_bits plat_clint_size(unit);

bool speculate_conditional(unit);
unit load_reservation(mach_bits);
bool match_reservation(mach_bits);
unit cancel_reservation(unit);

void plat_insns_per_tick(sail_int *rop, unit);

/* for syscall */
unit plat_term_write(mach_bits, mach_bits);
void plat_term_read(sail_string* zret_str, mach_bits file, mach_bits len);
/* for syscall */
mach_bits plat_htif_tohost(unit);
/* for syscall */
mach_bits plat_htif_fromhost(unit); 
/* for syscall */

/* for syscall */
mach_bits bits_of_int(sail_int num);
mach_bits bits_of_string(sail_string s, sail_int index);
/* for syscall */

unit memea(mach_bits, sail_int);
/* for syscall */
mach_bits plat_term_open(sail_int flag, sail_int mode);
mach_bits plat_term_close(mach_bits fd);
mach_bits plat_term_seek(mach_bits fd, mach_bits off, mach_bits dir);
unit plat_term_gettimeofday(unit u);
unit plat_term_times(unit u);
mach_bits plat_term_fstat(mach_bits fd);
mach_bits plat_term_utime(mach_bits atime, mach_bits mtime);

unit getChar2C(mach_bits);
/* for syscall */

/* for syscall */
void get_argv(sail_string* s, sail_int i);
/* for syscall */