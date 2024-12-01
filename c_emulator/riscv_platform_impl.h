#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Settings of the platform implementation. */

#define DEFAULT_RSTVEC 0x00001000

extern uint64_t rv_pmp_count;
extern uint64_t rv_pmp_grain;

extern uint64_t rv_vector_vlen_exp;
extern uint64_t rv_vector_elen_exp;

extern bool rv_enable_svinval;
extern bool rv_enable_zcb;
extern bool rv_enable_zfinx;
extern bool rv_enable_rvc;
extern bool rv_enable_fdext;
extern bool rv_enable_vext;
extern bool rv_enable_bext;
extern bool rv_enable_zicbom;
extern bool rv_enable_zicboz;
extern bool rv_enable_writable_misa;
extern bool rv_enable_dirty_update;
extern bool rv_enable_misaligned;
extern bool rv_mtval_has_illegal_inst_bits;
extern bool rv_enable_writable_fiom;
extern uint64_t rv_writable_hpm_counters;

extern uint64_t rv_ram_base;
extern uint64_t rv_ram_size;

extern uint64_t rv_rom_base;
extern uint64_t rv_rom_size;

extern uint64_t rv_cache_block_size_exp;

extern bool rv_vext_vl_use_ceil;

// Provides entropy for the scalar cryptography extension.
extern uint64_t rv_16_random_bits(void);

extern uint64_t rv_clint_base;
extern uint64_t rv_clint_size;

extern uint64_t rv_htif_tohost;
/* for syscall */
extern uint64_t rv_htif_fromhost; 
/* for syscall */
extern uint64_t rv_insns_per_tick;

extern FILE *trace_log;
extern int term_fd;
/* for syscall */

extern uint64_t zread_address;
extern uint64_t zstring_len;
extern uint64_t ztv_sec;
extern uint64_t ztv_usec;
extern uint64_t ztms_utime;


extern uint64_t zst_s;
extern uint64_t zst_blk;

extern uint64_t zst_dev;
extern uint64_t zst_ino;
extern uint64_t zst_mode;
extern uint64_t zst_nlink;
extern uint64_t zst_uid; 
extern uint64_t zst_gid; 
extern uint64_t zst_rdev; 
extern uint64_t zst_atim_sec; 
extern uint64_t zst_atim_nsec; 
extern uint64_t zst_mtim_sec; 
extern uint64_t zst_mtim_nsec; 
extern uint64_t zst_ctim_sec; 
extern uint64_t zst_ctim_nsec; 
extern uint64_t zst_blocks;
void plat_term_write_impl(int fd,char c);
void plat_term_read_impl(char** zret_str,int file, size_t len);
uint64_t plat_term_open_impl(char* name, int flag, int mode);
uint64_t plat_term_seek_impl(uint64_t fd, uint64_t off, uint64_t dir);
void plat_term_gettimeofday_impl();
void plat_term_times_impl();
uint64_t plat_term_close_impl(uint64_t fd);
uint64_t plat_term_fstat_impl(uint64_t fd);
uint64_t plat_term_utime_impl(char* name, uint64_t atime, uint64_t mtime);

int fd_alloc(int fd);
void fd_dealloc(int fd);
int fd_lookup(int fd);
/* for syscall */
