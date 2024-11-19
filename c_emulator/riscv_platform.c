#include "sail.h"
#include "rts.h"
#include "riscv_prelude.h"
#include "riscv_platform_impl.h"
#include "riscv_sail.h"
#include <string.h>
#ifdef DEBUG_RESERVATION
#include <stdio.h>
#include <inttypes.h>
#define RESERVATION_DBG(args...) fprintf(stderr, args)
#else
#define RESERVATION_DBG(args...)
#endif

/* This file contains the definitions of the C externs of Sail model. */

static mach_bits reservation = 0;
static bool reservation_valid = false;

bool sys_enable_rvc(unit u)
{
  return rv_enable_rvc;
}

bool sys_enable_fdext(unit u)
{
  return rv_enable_fdext;
}

bool sys_enable_svinval(unit u)
{
  return rv_enable_svinval;
}

bool sys_enable_zcb(unit u)
{
  return rv_enable_zcb;
}

bool sys_enable_zfinx(unit u)
{
  return rv_enable_zfinx;
}

bool sys_enable_writable_fiom(unit u)
{
  return rv_enable_writable_fiom;
}

bool sys_enable_vext(unit u)
{
  return rv_enable_vext;
}

bool sys_enable_bext(unit u)
{
  return rv_enable_bext;
}

bool sys_enable_zicbom(unit u)
{
  return rv_enable_zicbom;
}

bool sys_enable_zicboz(unit u)
{
  return rv_enable_zicboz;
}

uint64_t sys_pmp_count(unit u)
{
  return rv_pmp_count;
}

uint64_t sys_pmp_grain(unit u)
{
  return rv_pmp_grain;
}

uint64_t sys_vector_vlen_exp(unit u)
{
  return rv_vector_vlen_exp;
}

uint64_t sys_vector_elen_exp(unit u)
{
  return rv_vector_elen_exp;
}

bool sys_enable_writable_misa(unit u)
{
  return rv_enable_writable_misa;
}

mach_bits sys_writable_hpm_counters(unit u)
{
  return rv_writable_hpm_counters;
}

bool plat_enable_dirty_update(unit u)
{
  return rv_enable_dirty_update;
}

bool plat_enable_misaligned_access(unit u)
{
  return rv_enable_misaligned;
}

bool plat_mtval_has_illegal_inst_bits(unit u)
{
  return rv_mtval_has_illegal_inst_bits;
}

mach_bits plat_ram_base(unit u)
{
  return rv_ram_base;
}

mach_bits plat_ram_size(unit u)
{
  return rv_ram_size;
}

mach_bits plat_rom_base(unit u)
{
  return rv_rom_base;
}

mach_bits plat_rom_size(unit u)
{
  return rv_rom_size;
}

mach_bits plat_cache_block_size_exp()
{
  return rv_cache_block_size_exp;
}

// Provides entropy for the scalar cryptography extension.
mach_bits plat_get_16_random_bits(unit u)
{
  return rv_16_random_bits();
}

mach_bits plat_clint_base(unit u)
{
  return rv_clint_base;
}

mach_bits plat_clint_size(unit u)
{
  return rv_clint_size;
}

unit load_reservation(mach_bits addr)
{
  reservation = addr;
  reservation_valid = true;
  RESERVATION_DBG("reservation <- %0" PRIx64 "\n", reservation);
  return UNIT;
}

bool speculate_conditional(unit u)
{
  return true;
}

static mach_bits check_mask(void)
{
  return (zxlen_val == 32) ? 0x00000000FFFFFFFF : -1;
}

bool match_reservation(mach_bits addr)
{
  mach_bits mask = check_mask();
  bool ret = reservation_valid && (reservation & mask) == (addr & mask);
  RESERVATION_DBG("reservation(%c): %0" PRIx64 ", key=%0" PRIx64 ": %s\n",
                  reservation_valid ? 'v' : 'i', reservation, addr,
                  ret ? "ok" : "fail");
  return ret;
}

unit cancel_reservation(unit u)
{
  RESERVATION_DBG("reservation <- none\n");
  reservation_valid = false;
  return UNIT;
}

/* for syscall */
unit plat_term_write(mach_bits fd, mach_bits s)
{ 
  char c = s & 0xff;
  plat_term_write_impl(fd ,c);
  return UNIT;
}

void plat_term_read(sail_string* zret_str ,mach_bits file, mach_bits len)
{ 
  //fprintf(stderr, "plat_term_read = %d, %d\n", file, len);

  return plat_term_read_impl(zret_str, file, len);
}

char name_str[256] = {'\0'};
int name_index = 0;
unit getChar2C(mach_bits s)
{
  char c = s & 0xff;
  name_str[name_index++] = c;
  return UNIT;
}

mach_bits plat_term_open(sail_int flag, sail_int mode)
{
  int n = mpz_get_si(flag);
  int n2 = mpz_get_si(mode);

  int fd;
  //printf("%s\n", name_str);
  fd = plat_term_open_impl(name_str, n, n2);
  
  //初始化name_str和name_index 
  memset(name_str, '\0', sizeof(name_str));
  name_index = 0;
  return fd; 
}
mach_bits plat_term_close(mach_bits fd)
{
  return plat_term_close_impl(fd);
}

mach_bits plat_term_seek(mach_bits fd, mach_bits off, mach_bits dir)
{
  return plat_term_seek_impl(fd, off, dir);
}

unit plat_term_gettimeofday(unit u)
{
  plat_term_gettimeofday_impl();
  return UNIT;
}
unit plat_term_times(unit u)
{
  plat_term_times_impl();
  return UNIT;
}

mach_bits plat_term_fstat(mach_bits fd)
{
  return plat_term_fstat_impl(fd);
}

mach_bits plat_term_utime(mach_bits atime, mach_bits mtime)
{
  mach_bits ret;

  ret = plat_term_utime_impl(name_str,atime, mtime);
  memset(name_str, '\0', sizeof(name_str));
  name_index = 0;
  
  return ret;
}
/* for syscall */

void plat_insns_per_tick(sail_int *rop, unit u)
{ }

mach_bits plat_htif_tohost(unit u)
{
  return rv_htif_tohost;
}
//typedef uint64_t mach_bits; 
/* for syscall */
mach_bits plat_htif_fromhost(unit u)
{
  //fprintf(stderr, "test fromhost\n");

  return rv_htif_fromhost;
}
/* for syscall */

unit memea(mach_bits len, sail_int n)
{
  return UNIT;
}

/* for syscall */
mach_bits bits_of_string(sail_string s, sail_int index) 
{
  int index2 = mpz_get_si(index);

  return s[index2];
}

mach_bits bits_of_int(sail_int num)
{
  int num2 = mpz_get_si(num);
  return num2;
}
/* for syscall */