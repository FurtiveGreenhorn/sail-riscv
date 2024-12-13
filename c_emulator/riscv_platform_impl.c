#include "riscv_platform_impl.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <utime.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/* for syscall */
//CLOCK_MONOTONIC 自系統啟動以來的到現在的時間
//CLOCK_REALTIME 自1970年1月u號以來到現在的時間

#if defined(__APPLE__)
#define HAVE_MACH_TIMER
#include <mach/mach_time.h>
#elif !defined(_WIN32) && !defined(_WIN64) 
#define HAVE_POSIX_TIMER
#ifdef CLOCK_MONOTONIC 
#define CLOCKID CLOCK_MONOTONIC 
#else
#define CLOCKID CLOCK_REALTIME
#endif
#endif

#define FD_SIZE 25
/* for syscall */


/* Settings of the platform implementation, with common defaults. */
uint64_t rv_pmp_count = 0;
uint64_t rv_pmp_grain = 0;

uint64_t rv_vector_vlen_exp = 0x9;
uint64_t rv_vector_elen_exp = 0x6;

bool rv_enable_svinval = false;
bool rv_enable_zcb = false;
bool rv_enable_zfinx = false;
bool rv_enable_rvc = true;
bool rv_enable_writable_misa = true;
bool rv_enable_fdext = true;
bool rv_enable_vext = true;
bool rv_enable_bext = false;
bool rv_enable_zicbom = false;
bool rv_enable_zicboz = false;
bool rv_enable_sstc = false;

bool rv_enable_dirty_update = false;
bool rv_enable_misaligned = false;
bool rv_mtval_has_illegal_inst_bits = false;
bool rv_enable_writable_fiom = true;
uint64_t rv_writable_hpm_counters = 0xFFFFFFFF;

uint64_t rv_ram_base = UINT64_C(0x80000000);
uint64_t rv_ram_size = UINT64_C(0x80000000);

uint64_t rv_rom_base = UINT64_C(0x1000);
uint64_t rv_rom_size = UINT64_C(0x100);

bool rv_vext_vl_use_ceil = false;

// Default 64, which is mandated by RVA22.
uint64_t rv_cache_block_size_exp = UINT64_C(6);

// Provides entropy for the scalar cryptography extension.
uint64_t rv_16_random_bits(void)
{
  // This function can be changed to support deterministic sequences of
  // pseudo-random bytes. This is useful for testing.
  const char *name = "/dev/urandom";
  FILE *f = fopen(name, "rb");
  uint16_t val;
  if (fread(&val, 2, 1, f) != 1) {
    fprintf(stderr, "Unable to read 2 bytes from %s\n", name);
  }
  fclose(f);
  return (uint64_t)val;
}

uint64_t rv_clint_base = UINT64_C(0x2000000);
uint64_t rv_clint_size = UINT64_C(0xc0000);

/* for syscall */
uint64_t rv_htif_tohost = UINT64_C(0x80002700);   
uint64_t rv_htif_fromhost = UINT64_C(0x80002708); 
/* for syscall */
uint64_t rv_insns_per_tick = UINT64_C(100);

int term_fd = 1; // set during startup
/* for syscall */
static int fds[FD_SIZE] = {0,1,2};

int fd_alloc(int fd)
{
  int i =3;
  
  for(; i<FD_SIZE; i++){
    if(fds[i] == 0){
      fds[i] = fd;
      break;
    }
  } 

  return i;
}

void fd_dealloc(int fd)
{
  fds[fd] = 0;
}

int fd_lookup(int fd)
{
  return fd >= FD_SIZE ? -1: fds[fd];
}

/*
enum {
  O_RDONLY = 0,
  O_WRONLY = 1,
  O_RDWR =   2,
  O_ACCMODE = 3,
};
*/

// ntust
// O_ACCMODE為特殊flag，O_ACCMODE = O_RDONLY | O_WRONLY | O_RDWR ，利用遮罩來取得實際的O_RDONLY O_WRONLY O_RDWR
// 一定要做轉換 因為回傳的flags 不會是0 1 2 而是更大的數字
int get_mode_str(uint32_t flags)
{
    switch (flags & O_ACCMODE) {
    case O_RDONLY:
        return O_RDONLY;
    case O_WRONLY:
        return O_WRONLY | O_CREAT;
    case O_RDWR:
        return O_RDWR | O_CREAT;
    default:
        return -1;
    }
}

void plat_term_write_impl(int fd, char c)
{
  if (write(fd_lookup(fd), &c, sizeof(c)) < 0) {
    fprintf(stderr, "Unable to write to terminal!\n");
  }
}

void plat_term_read_impl(char** zret_str, int file, size_t len)
{  
  //int ret;
  char *s;
  s = (char*)malloc(len);
  memset(s, 0, len);
  
  zstring_len = read(fd_lookup(file), s, len);

  /*
  int pos;
  pos = lseek(fd_lookup(file), 0, SEEK_CUR);
  printf("pos = %d\n", pos);
  */
  //printf("-------------------------------------------------------------------------------\n");
/*
  printf("here_ret-------------------- = %d\n", ret);
  printf("%.*s\n",  1023,s);
  printf("%s\n", s);
*/
  if (zstring_len < 0) {
    fprintf(stderr, "Unable to read to terminal!\n");
  }
  * zret_str = s;
  return;
}

uint64_t plat_term_open_impl(char* name,int flag, int mode)
{
  /*
  FILE* fp;
  fp = fopen(name , get_mode_str(flag));
  
  if(fp == NULL){
    fprintf(stderr, "Unable to open the file\n");
    return -1;
  }
  
  int fd = fileno(fp);
  //printf("original fd = %d\n", fd);
  return fd_alloc(fd);
  */
  int fd;
  if(get_mode_str(flag) == O_RDONLY )
    fd = open(name, get_mode_str(flag));
  else
    fd = open(name, get_mode_str(flag), 0666);

  return fd_alloc(fd);

}

uint64_t plat_term_close_impl(uint64_t fd){
  if(fd >=3){
    
    int n = close(fd_lookup(fd));
    fd_dealloc(fd);
    
    if(n == -1){
      fprintf(stderr, "Unable to close the file\n");
     return -1;
    }
    return 0; 
  }
  
  return 0; 
}

uint64_t plat_term_seek_impl(uint64_t fd, uint64_t off, uint64_t dir)
{  
    int n = lseek(fd_lookup(fd), off, dir);
   // printf("ofd = %d fd = %d off = %d dir = %d n = %d\n", fd_lookup(fd), fd, off, dir, n);

    if(n == -1){
        fprintf(stderr, "Unable to seek the file\n");
        return -1;
    }
    return n;
}
void plat_term_gettimeofday_impl()
{
  //  #if HAVE_POSIX_TIMER 這樣寫, HAVE_POSIX_TIMER為0會不成立

  #if defined(HAVE_POSIX_TIMER)
    struct timespec t;
    clock_gettime(CLOCKID, &t);
    ztv_sec = t.tv_sec;
    ztv_usec = t.tv_nsec / 1000;
  #else /* low resolution timer */
    clock_t t = clock();
    ztv_sec = t / CLOCKS_PER_SEC;
    ztv_usec = (t % CLOCKS_PER_SEC) * (1e6 / CLOCKS_PER_SEC);
  #endif

}
void plat_term_times_impl()
{
  static char initialized;
  static struct timeval t0;
  struct timeval t;

  gettimeofday (&t, 0);

  // when called for the first time, initialize t0
  if (!initialized) {
    t0.tv_sec = t.tv_sec;
    t0.tv_usec = t.tv_usec;
    initialized = 1;
  }

  long long utime = (t.tv_sec - t0.tv_sec) * 1000000 + (t.tv_usec - t0.tv_usec);
  ztms_utime = utime * CLOCKS_PER_SEC / 1000000;
  //參考https://github.com/bminor/newlib/blob/master/libgloss/riscv/sys_times.c?fbclid=IwAR1z4Gr4oqBqP1xzOyz3BIjQArDMmETSg_juOpR8OpF4WmEvC4jOvFcqFy0#L22
}
uint64_t plat_term_fstat_impl(uint64_t fd)
{
   uint64_t ret;
   struct stat fileStat;
   ret = fstat(fd_lookup(fd), &fileStat);
   
   zst_dev = (long)fileStat.st_dev;
   zst_ino = (long)fileStat.st_ino;
   zst_mode = (unsigned int)fileStat.st_mode;
   zst_nlink = (long)fileStat.st_nlink;
   zst_uid = (long)fileStat.st_uid; 
   zst_gid = (long)fileStat.st_gid; 
   zst_rdev = (long)fileStat.st_rdev; 
   zst_s = (long)fileStat.st_size;
   zst_atim_sec = fileStat.st_atim.tv_sec; 
   zst_atim_nsec = fileStat.st_atim.tv_nsec; 
   zst_mtim_sec = fileStat.st_mtim.tv_sec; 
   zst_mtim_nsec = fileStat.st_mtim.tv_nsec; 
   zst_ctim_sec = fileStat.st_ctim.tv_sec; 
   zst_ctim_nsec = fileStat.st_ctim.tv_nsec; 
   zst_blk = (long)fileStat.st_blksize; 
   zst_blocks = (long)fileStat.st_blocks;
   /*
   printf("sail simulator---------------------------\n");
   printf("Device: \t\t%ld\n", (long)fileStat.st_dev);
   printf("Inode: \t\t\t%ld\n", (long)fileStat.st_ino);
   printf("Mode: \t\t\t%o (octal)\n", (unsigned int)fileStat.st_mode);
   printf("Number of Links: \t%ld\n", (long)fileStat.st_nlink);
   printf("User ID: \t\t%ld\n", (long)fileStat.st_uid);
   printf("Group ID: \t\t%ld\n", (long)fileStat.st_gid);
   printf("Device ID (if special file): \t%ld\n", (long)fileStat.st_rdev);
   printf("Total Size: \t\t%ld bytes\n", (long)fileStat.st_size);
   printf("Block Size: \t\t%ld bytes\n", (long)fileStat.st_blksize);
   printf("Number of Blocks: \t%ld\n", (long)fileStat.st_blocks);
   printf("Last Access Time: \t%s", ctime(&fileStat.st_atim.tv_sec));
   printf("Last Modification Time: \t%s", ctime(&fileStat.st_mtim.tv_sec));
   printf("Last Status Change Time: \t%s", ctime(&fileStat.st_ctim.tv_sec));
   */

   return ret;
}

uint64_t plat_term_utime_impl(char* name, uint64_t atime, uint64_t mtime)
{
  uint64_t ret;
  struct utimbuf new_times;
  //struct stat file_stat;
  //printf("test = %s\n", name);
  new_times.actime = atime;
  new_times.modtime = mtime;
  
  //file_stat

  ret = utime(name, &new_times);
}
/* for syscall */