/*=======================================================================================*/
/*  This Sail RISC-V architecture model, comprising all files and                        */
/*  directories except where otherwise noted is subject the BSD                          */
/*  two-clause license in the LICENSE file.                                              */
/*                                                                                       */
/*  SPDX-License-Identifier: BSD-2-Clause                                                */
/*=======================================================================================*/

/* Platform-specific definitions, and basic MMIO devices. */

/* Current constraints on this implementation are:
   - it cannot access memory directly, but instead provides definitions for the physical memory model
   - it can access system register state, needed to manipulate interrupt bits
   - it relies on externs to get platform address information and doesn't hardcode them
*/

val elf_tohost = pure {
  interpreter: "Elf_loader.elf_tohost",
  c: "elf_tohost"
} :  unit -> int

val elf_entry = pure {
  interpreter: "Elf_loader.elf_entry",
  c: "elf_entry"
} : unit -> int

// Cache block size is 2^cache_block_size_exp. Max is `max_mem_access` (4096)
// because this model performs `cbo.zero` with a single write, and the behaviour
// with cache blocks larger than a page is not clearly defined.
val plat_cache_block_size_exp = pure {c: "plat_cache_block_size_exp", interpreter: "Platform.cache_block_size_exp", lem: "plat_cache_block_size_exp"} : unit -> range(0, 12)

/* Main memory */
val plat_ram_base = pure {c: "plat_ram_base", interpreter: "Platform.dram_base", lem: "plat_ram_base"} : unit -> physaddrbits
val plat_ram_size = pure {c: "plat_ram_size", interpreter: "Platform.dram_size", lem: "plat_ram_size"} : unit -> physaddrbits

/* whether the MMU should update dirty bits in PTEs */
val plat_enable_dirty_update = pure {interpreter: "Platform.enable_dirty_update",
                                     c: "plat_enable_dirty_update",
                                     lem: "plat_enable_dirty_update"} : unit -> bool

/* whether the platform supports misaligned accesses without trapping to M-mode. if false,
 * misaligned loads/stores are trapped to Machine mode.
 */
val plat_enable_misaligned_access = pure {interpreter: "Platform.enable_misaligned_access",
                                          c: "plat_enable_misaligned_access",
                                          lem: "plat_enable_misaligned_access"} : unit -> bool

/* whether mtval stores the bits of a faulting instruction on illegal instruction exceptions */
val plat_mtval_has_illegal_inst_bits = pure {interpreter: "Platform.mtval_has_illegal_inst_bits",
                                             c: "plat_mtval_has_illegal_inst_bits",
                                             lem: "plat_mtval_has_illegal_inst_bits"} : unit -> bool

/* ROM holding reset vector and device-tree DTB */
val plat_rom_base   = pure {interpreter: "Platform.rom_base", c: "plat_rom_base", lem: "plat_rom_base"} : unit -> physaddrbits
val plat_rom_size   = pure {interpreter: "Platform.rom_size", c: "plat_rom_size", lem: "plat_rom_size"} : unit -> physaddrbits

/* Location of clock-interface, which should match with the spec in the DTB */
val plat_clint_base = pure {interpreter: "Platform.clint_base", c: "plat_clint_base", lem: "plat_clint_base"} : unit -> physaddrbits
val plat_clint_size = pure {interpreter: "Platform.clint_size", c: "plat_clint_size", lem: "plat_clint_size"} : unit -> physaddrbits

/* for syscall */
//sys_read
register string_len : xlenbits

//sys_getmainvars
register htif_argc : int


//sys_gettimeofday
register tv_sec : xlenbits
register tv_usec : xlenbits

//sys_times
//utime = user time, tms = a c structure
register tms_utime : xlenbits

//sys_fstat
//這兩個用原本的名稱會出錯
register st_s : xlenbits
register st_blk : xlenbits
//
register st_dev : bits(16)
register st_ino : bits(16)
register st_mode : bits(32)
register st_nlink : bits(16)
register st_uid : bits(16)
register st_gid : bits(16)
register st_rdev : bits(16)
register st_atim_sec : xlenbits
register st_atim_nsec : xlenbits
register st_mtim_sec : xlenbits
register st_mtim_nsec : xlenbits
register st_ctim_sec : xlenbits
register st_ctim_nsec : xlenbits
register st_blocks : xlenbits
val get_argv = pure {c: "get_argv" } : int -> string
val plat_htif_fromhost = impure {c: "plat_htif_fromhost"} : unit -> xlenbits 
val bits_of_string = pure {c: "bits_of_string"} : (string, int) -> bits(8) 
val bits_of_int = pure {c: "bits_of_int"} : (int) -> xlenbits 

function int_of_bits(xs: xlenbits) -> nat = {
	var acc : nat = 0;
  let bound = sizeof(xlen) - 1;
	foreach(i from 0 to bound) {
		acc = acc * 2;
		if bit_to_bool(xs[(bound - i)])
		then acc = acc + 1
	};
	acc
}
/* for syscall */

/* Location of HTIF ports */
val plat_htif_tohost = pure {c: "plat_htif_tohost", lem: "plat_htif_tohost"} : unit -> physaddrbits
function plat_htif_tohost () = to_bits(physaddrbits_len, elf_tohost ())
// todo: fromhost


val phys_mem_segments : unit -> list((physaddrbits, physaddrbits))
function phys_mem_segments() =
  (plat_rom_base (), plat_rom_size ()) ::
  (plat_ram_base (), plat_ram_size ()) ::
  [||]

/* Physical memory map predicates */

function within_phys_mem forall 'n, 'n <= max_mem_access. (physaddr(addr) : physaddr, width : int('n)) -> bool = {
  /* To avoid overflow issues when physical memory extends to the end
   * of the addressable range, we need to perform address bound checks
   * on unsigned unbounded integers.
   */
  let addr_int     = unsigned(addr);
  let ram_base_int = unsigned(plat_ram_base ());
  let rom_base_int = unsigned(plat_rom_base ());
  let ram_size_int = unsigned(plat_ram_size ());
  let rom_size_int = unsigned(plat_rom_size ());

  /* todo: iterate over segment list */
  if      (  ram_base_int <= addr_int
           & (addr_int + sizeof('n)) <= (ram_base_int + ram_size_int))
  then    true
  else if (  rom_base_int <= addr_int
           & (addr_int + sizeof('n)) <= (rom_base_int + rom_size_int))
  then    true
  else {
    print_platform("within_phys_mem: " ^ BitStr(addr) ^ " not within phys-mem:");
    print_platform("  plat_rom_base: " ^ BitStr(plat_rom_base ()));
    print_platform("  plat_rom_size: " ^ BitStr(plat_rom_size ()));
    print_platform("  plat_ram_base: " ^ BitStr(plat_ram_base ()));
    print_platform("  plat_ram_size: " ^ BitStr(plat_ram_size ()));
    false
  }
}

function within_clint forall 'n, 0 < 'n <= max_mem_access . (physaddr(addr) : physaddr, width : int('n)) -> bool = {
  /* To avoid overflow issues when physical memory extends to the end
   * of the addressable range, we need to perform address bound checks
   * on unsigned unbounded integers.
   */
  let addr_int       = unsigned(addr);
  let clint_base_int = unsigned(plat_clint_base ());
  let clint_size_int = unsigned(plat_clint_size ());
    clint_base_int <= addr_int
  & (addr_int + sizeof('n)) <= (clint_base_int + clint_size_int)
}

function within_htif_writable forall 'n, 0 < 'n <= max_mem_access . (physaddr(addr) : physaddr, width : int('n)) -> bool =
    plat_htif_tohost() == addr | (plat_htif_tohost() + 4 == addr & width == 4)

function within_htif_readable forall 'n, 0 < 'n <= max_mem_access . (physaddr(addr) : physaddr, width : int('n)) -> bool =
    plat_htif_tohost() == addr | (plat_htif_tohost() + 4 == addr & width == 4)
/* for syscall */
function within_htif_fromhost_writable forall 'n, 0 < 'n <= max_mem_access . (addr : xlenbits, width : atom('n)) -> bool =
    plat_htif_fromhost() == addr | (plat_htif_fromhost() + 4 == addr & width == 4)

function within_htif_fromhost_readable forall 'n, 0 < 'n <= max_mem_access . (addr : xlenbits, width : atom('n)) -> bool =
    plat_htif_fromhost() == addr | (plat_htif_fromhost() + 4 == addr & width == 4)
/* for syscall */

/* CLINT (Core Local Interruptor), based on Spike. */

val plat_insns_per_tick = pure {interpreter: "Platform.insns_per_tick", c: "plat_insns_per_tick", lem: "plat_insns_per_tick"} : unit -> int

// Each hart has a memory-mapped mtimecmp register. Typically these are
// exposed as an array in CLINT. The CLINT implementation here is currently
// hard-coded to use the mtimecmp for hart 0.
register mtimecmp : bits(64)

// Unlike mtimecmp, stimecmp is a real CSR; not memory mapped.
register stimecmp : bits(64)

/* CLINT memory-mapped IO */

/* relative address map:
 *
 * 0000 msip hart 0         -- memory-mapped software interrupt
 * 0004 msip hart 1
 * 4000 mtimecmp hart 0 lo  -- memory-mapped timer thresholds
 * 4004 mtimecmp hart 0 hi
 * 4008 mtimecmp hart 1 lo
 * 400c mtimecmp hart 1 hi
 * bff8 mtime lo            -- memory-mapped clocktimer value
 * bffc mtime hi
 */

let MSIP_BASE        : physaddrbits = zero_extend(0x00000)
let MTIMECMP_BASE    : physaddrbits = zero_extend(0x04000)
let MTIMECMP_BASE_HI : physaddrbits = zero_extend(0x04004)
let MTIME_BASE       : physaddrbits = zero_extend(0x0bff8)
let MTIME_BASE_HI    : physaddrbits = zero_extend(0x0bffc)

val clint_load : forall 'n, 'n > 0. (AccessType(ext_access_type), physaddr, int('n)) -> MemoryOpResult(bits(8 * 'n))
function clint_load(t, physaddr(addr), width) = {
  let addr = addr - plat_clint_base ();
  /* FIXME: For now, only allow exact aligned access. */
  if addr == MSIP_BASE & ('n == 8 | 'n == 4)
  then {
    if   get_config_print_platform()
    then print_platform("clint[" ^ BitStr(addr) ^ "] -> " ^ BitStr(mip[MSI]));
    MemValue(sail_zero_extend(mip[MSI], sizeof(8 * 'n)))
  }
  else if addr == MTIMECMP_BASE & ('n == 4)
  then {
    if   get_config_print_platform()
    then print_platform("clint<4>[" ^ BitStr(addr) ^ "] -> " ^ BitStr(mtimecmp[31..0]));
    /* FIXME: Redundant zero_extend currently required by Lem backend */
    MemValue(sail_zero_extend(mtimecmp[31..0], 32))
  }
  else if addr == MTIMECMP_BASE & ('n == 8)
  then {
    if   get_config_print_platform()
    then print_platform("clint<8>[" ^ BitStr(addr) ^ "] -> " ^ BitStr(mtimecmp));
    /* FIXME: Redundant zero_extend currently required by Lem backend */
    MemValue(sail_zero_extend(mtimecmp, 64))
  }
  else if addr == MTIMECMP_BASE_HI & ('n == 4)
  then {
    if   get_config_print_platform()
    then print_platform("clint-hi<4>[" ^ BitStr(addr) ^ "] -> " ^ BitStr(mtimecmp[63..32]));
    /* FIXME: Redundant zero_extend currently required by Lem backend */
    MemValue(sail_zero_extend(mtimecmp[63..32], 32))
  }
  else if addr == MTIME_BASE & ('n == 4)
  then {
    if   get_config_print_platform()
    then print_platform("clint[" ^ BitStr(addr) ^ "] -> " ^ BitStr(mtime));
    MemValue(sail_zero_extend(mtime[31..0], 32))
  }
  else if addr == MTIME_BASE & ('n == 8)
  then {
    if   get_config_print_platform()
    then print_platform("clint[" ^ BitStr(addr) ^ "] -> " ^ BitStr(mtime));
    MemValue(sail_zero_extend(mtime, 64))
  }
  else if addr == MTIME_BASE_HI & ('n == 4)
  then {
    if   get_config_print_platform()
    then print_platform("clint[" ^ BitStr(addr) ^ "] -> " ^ BitStr(mtime));
    MemValue(sail_zero_extend(mtime[63..32], 32))
  }
  else {
    if   get_config_print_platform()
    then print_platform("clint[" ^ BitStr(addr) ^ "] -> <not-mapped>");
    match t {
      Execute()  => MemException(E_Fetch_Access_Fault()),
      Read(Data) => MemException(E_Load_Access_Fault()),
      _          => MemException(E_SAMO_Access_Fault())
    }
  }
}

function clint_dispatch() -> unit = {
  mip[MTI] = bool_to_bits(mtimecmp <=_u mtime);
  if extensionEnabled(Ext_Sstc) then {
    mip[STI] = bool_to_bits(stimecmp <=_u mtime);
  };
  if get_config_print_platform()
  then print_platform("clint mtime " ^ BitStr(mtime) ^ " (mip.MTI <- " ^ BitStr(mip[MTI]) ^
    (if extensionEnabled(Ext_Sstc) then ", mip.STI <- " ^ BitStr(mip[STI]) else "") ^ ")");
}

/* The rreg effect is due to checking mtime. */
val clint_store: forall 'n, 'n > 0. (physaddr, int('n), bits(8 * 'n)) -> MemoryOpResult(bool)
function clint_store(physaddr(addr), width, data) = {
  let addr = addr - plat_clint_base ();
  if addr == MSIP_BASE & ('n == 8 | 'n == 4) then {
    if   get_config_print_platform()
    then print_platform("clint[" ^ BitStr(addr) ^ "] <- " ^ BitStr(data) ^ " (mip.MSI <- " ^ BitStr(data[0]) ^ ")");
    mip[MSI] = [data[0]];
    clint_dispatch();
    MemValue(true)
  } else if addr == MTIMECMP_BASE & 'n == 8 then {
    if   get_config_print_platform()
    then print_platform("clint<8>[" ^ BitStr(addr) ^ "] <- " ^ BitStr(data) ^ " (mtimecmp)");
    mtimecmp = sail_zero_extend(data, 64); /* FIXME: Redundant zero_extend currently required by Lem backend */
    clint_dispatch();
    MemValue(true)
  } else if addr == MTIMECMP_BASE & 'n == 4 then {
    if   get_config_print_platform()
    then print_platform("clint<4>[" ^ BitStr(addr) ^ "] <- " ^ BitStr(data) ^ " (mtimecmp)");
    mtimecmp = vector_update_subrange(mtimecmp, 31, 0, sail_zero_extend(data, 32));  /* FIXME: Redundant zero_extend currently required by Lem backend */
    clint_dispatch();
    MemValue(true)
  } else if addr == MTIMECMP_BASE_HI & 'n == 4 then {
    if   get_config_print_platform()
    then print_platform("clint<4>[" ^ BitStr(addr) ^ "] <- " ^ BitStr(data) ^ " (mtimecmp)");
    mtimecmp = vector_update_subrange(mtimecmp, 63, 32, sail_zero_extend(data, 32)); /* FIXME: Redundant zero_extend currently required by Lem backend */
    clint_dispatch();
    MemValue(true)
  } else if addr == MTIME_BASE & 'n == 8 then {
    if   get_config_print_platform()
    then print_platform("clint<8>[" ^ BitStr(addr) ^ "] <- " ^ BitStr(data) ^ " (mtime)");
    mtime = data;
    clint_dispatch();
    MemValue(true)
  } else if addr == MTIME_BASE & 'n == 4 then {
    if   get_config_print_platform()
    then print_platform("clint<4>[" ^ BitStr(addr) ^ "] <- " ^ BitStr(data) ^ " (mtime)");
    mtime[31 .. 0] = data;
    clint_dispatch();
    MemValue(true)
  } else if addr == MTIME_BASE_HI & 'n == 4 then {
    if   get_config_print_platform()
    then print_platform("clint<4>[" ^ BitStr(addr) ^ "] <- " ^ BitStr(data) ^ " (mtime)");
    mtime[63 .. 32] = data;
    clint_dispatch();
    MemValue(true)
  } else {
    if   get_config_print_platform()
    then print_platform("clint[" ^ BitStr(addr) ^ "] <- " ^ BitStr(data) ^ " (<unmapped>)");
    MemException(E_SAMO_Access_Fault())
  }
}

val tick_clock : unit -> unit
function tick_clock() = {
  if   mcountinhibit[CY] == 0b0
  then mcycle = mcycle + 1;

  mtime  = mtime  + 1;
  clint_dispatch()
}

/* Basic terminal character I/O. */

/* for syscall */
val getChar2C = pure {c: "getChar2C"} : bits(8) -> unit
val plat_term_write = impure {ocaml: "Platform.term_write", c: "plat_term_write", lem: "plat_term_write"} : (bits(64) ,bits(8)) -> unit
val plat_term_open = impure {c: "plat_term_open"} : (int, int) -> xlenbits 
val plat_term_read  = impure {c: "plat_term_read"}  : (bits(64), bits(64)) -> string
val plat_term_close  = impure {c: "plat_term_close"}  : bits(64) -> xlenbits
val plat_term_seek  = impure {c: "plat_term_seek"}  :(bits(64), bits(64), bits(64)) -> xlenbits
val plat_term_gettimeofday = pure {c: "plat_term_gettimeofday"} : unit -> unit
val plat_term_times = pure {c: "plat_term_times"} : unit -> unit
val plat_term_fstat = pure {c: "plat_term_fstat"} : (bits(64)) -> xlenbits
val plat_term_utime = pure {c: "plat_term_utime"} : (bits(64), bits(64)) -> xlenbits
/* for syscall */

/* Spike's HTIF device interface, which multiplexes the above MMIO devices. */

bitfield htif_cmd : bits(64) = {
  device  : 63 .. 56,
  cmd     : 55 .. 48,
  payload : 47 .. 0
}

register htif_tohost : bits(64)
register htif_done   : bool
register htif_exit_code : bits(64)

/* Applications sometimes write the lower 32-bit payload bytes without
   writing the control bytes; this is seen in the riscv-tests suite.
   However, processing the payload bytes too early could miss a
   subsequent write to the control bytes.  As a workaround, if the
   payload is written a few times with the same value, without an
   intervening write to the control bytes, we process the whole htif
   command anyway.  */

register htif_cmd_write : bit
register htif_payload_writes : bits(4)

/* Once a htif command has been processed, the port is reset. */
function reset_htif () -> unit = {
  htif_cmd_write = bitzero;
  htif_payload_writes = 0x0;
  htif_tohost = zeros();
}

/* Since the htif tohost port is only available at a single address,
 * we'll assume here that physical memory model has correctly
 * dispatched the address.
 */

val htif_load : forall 'n, 'n > 0. (AccessType(ext_access_type), physaddr, int('n)) -> MemoryOpResult(bits(8 * 'n))
function htif_load(t, physaddr(paddr), width) = {
  if   get_config_print_platform()
  then print_platform("htif[" ^ BitStr(paddr) ^ "] -> " ^ BitStr(htif_tohost));
  /* FIXME: For now, only allow the expected access widths. */
  if      width == 8 & (paddr == plat_htif_tohost())
  then    MemValue(sail_zero_extend(htif_tohost, 64))         /* FIXME: Redundant zero_extend currently required by Lem backend */
  else if width == 4 & paddr == plat_htif_tohost()
  then    MemValue(sail_zero_extend(htif_tohost[31..0], 32))  /* FIXME: Redundant zero_extend currently required by Lem backend */
  else if width == 4 & paddr == plat_htif_tohost() + 4
  then    MemValue(sail_zero_extend(htif_tohost[63..32], 32)) /* FIXME: Redundant zero_extend currently required by Lem backend */
  else match t {
    Execute()  => MemException(E_Fetch_Access_Fault()),
    Read(Data) => MemException(E_Load_Access_Fault()),
    _          => MemException(E_SAMO_Access_Fault())
  }
}

/* The rreg,wreg effects are an artifact of using 'register' to implement device state. */
val htif_store: forall 'n, 0 < 'n <= 8. (physaddr, int('n), bits(8 * 'n)) -> MemoryOpResult(bool)
function htif_store(physaddr(paddr), width, data) = {
  if   get_config_print_platform()
  then print_platform("htif[" ^ BitStr(paddr) ^ "] <- " ^ BitStr(data));
  /* Store the written value so that we can ack it later. */
  if      width == 8
  then    { htif_cmd_write = bitone;
            htif_payload_writes = htif_payload_writes + 1;
            htif_tohost = zero_extend(data) }
  else if width == 4 & paddr == plat_htif_tohost()
  then    { if   data == htif_tohost[31 .. 0]
            then htif_payload_writes = htif_payload_writes + 1
            else htif_payload_writes = 0x1;
            htif_tohost = vector_update_subrange(htif_tohost, 31, 0, data) }
  else if width == 4 & paddr == plat_htif_tohost() + 4
  then    { if   data[15 .. 0] == htif_tohost[47 .. 32]
            then htif_payload_writes = htif_payload_writes + 1
            else htif_payload_writes = 0x1;
            htif_cmd_write = bitone;
            htif_tohost = vector_update_subrange(htif_tohost, 63, 32, data) }
  /* unaligned command writes are not supported and will not be detected */
  else    { htif_tohost = zero_extend(data) };

  /* Execute if there were repeated writes of the same payload without
   * a cmd (e.g. in riscv-tests), or we have a complete htif command.
   */
  if   (((htif_cmd_write == bitone) & (unsigned(htif_payload_writes) > 0))
        | (unsigned(htif_payload_writes) > 2))
  then {
    let cmd = Mk_htif_cmd(htif_tohost);
    /* for sysycall */
    //print("htif_tohost: " ^ BitStr(htif_tohost));

    /*=== get system number and print it ===*/
    let syscall_number_tuple = read_ram(Read_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8, false);
    let (syscall_number_bits_const  , _) = syscall_number_tuple;
    let syscall_number_bits_var : xlenbits = syscall_number_bits_const;
    let syscall_number : int = int_of_bits(syscall_number_bits_var);
    //print("syscall_number_bits: " ^ BitStr(syscall_number_bits_var));
    //print_int("syscall_number:  ", syscall_number);
    /*======================================*/

    /*=== get a0  ===*/
    let a0_tuple = read_ram(Read_plain, physaddr(sail_zero_extend(cmd[payload] + 8, 64)), 8, false);
    let (a0_bits_const  , _) = a0_tuple;
    let a0_bits_var : xlenbits = a0_bits_const;
    let a0 : int = int_of_bits(a0_bits_var);
    //print("a0_bits: " ^ BitStr(a0_bits_var));
    //print_int("a0:  ", a0);
    /*===============*/
    /*=== get a1  ===*/
    let a1_tuple = read_ram(Read_plain, physaddr(sail_zero_extend(cmd[payload] + 16, 64)), 8, false);
    let (a1_bits_const  , _) = a1_tuple;
    let a1_bits_var : xlenbits = a1_bits_const;
    let a1 : int = int_of_bits(a1_bits_var);
    //print("a1_bits: " ^ BitStr(a1_bits_var));
    //print_int("a1:  ", a1);
    /*===============*/
    /*=== get a2  ===*/
    let a2_tuple = read_ram(Read_plain, physaddr(sail_zero_extend(cmd[payload] + 24, 64)), 8, false);
    let (a2_bits_const  , _) = a2_tuple;
    let a2_bits_var : xlenbits = a2_bits_const;
    let a2 : int = int_of_bits(a2_bits_var);
    //print("a2_bits: " ^ BitStr(a2_bits_var));
    //print_int("a2:  ", a2);
    /*=============-==*/

    match syscall_number {
      /*=== sys_close ===*/
        57 => {
          let n = plat_term_close(a0_bits_var);
          //print("check close file = " ^ BitStr(n));
        },
        /*=== sys_lseek ===*/
        62 => {
          let n = plat_term_seek(a0_bits_var,a1_bits_var,a2_bits_var);
        },
        /*=== sys_read ===*/
        63 => {
        
          let ret_string = plat_term_read(a0_bits_var, a2_bits_var);
          //print("ret_string = " ^ ret_string);
          // print_int("string_length = " , string_len);
          if string_len == sign_extend(0b1)
          then {
            let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8, string_len, ());
          }
          else {
            let  str_len = int_of_bits(string_len);
/*
            var vec_argv_count : int = 0;
            if emod_int(string_length(vec_argv[i]) + 1, 8) == 0  
            then vec_argv_count = ediv_int(sub_int(string_length(vec_argv[i]) + 1, 1),8)
            else vec_argv_count = ediv_int(string_length(vec_argv[i]) + 1,8);
*/
            foreach(i from 0 to ediv_int(str_len, 8)){
                var append_str : bits(64) = zero_extend(0b0);
                var len_copy : int = 0;
                if str_len - (i * 8) > 8  
                then len_copy = 8
                else len_copy = (str_len - (i * 8));
                foreach(j from 0 to (len_copy - 1)){
                  let bos = bits_of_string(ret_string, j + i*8);
                  if j < 8
                  then append_str = vector_update_subrange(append_str, j*8+7 ,j*8, bos); 
                  //print("append_str = " ^ BitStr(append_str));
                };
                let _ = write_ram(Write_plain, physaddr(a1_bits_var + i * 8), 8, append_str, ());
              
            };
            //string_length(ret_string)回傳為int，需呼叫bits_of_int來轉成bitvector
            //print("ret_string: " ^ BitStr(bits_of_int(string_length(ret_string))));
            let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8,  string_len, ());
          }
        },
                  
        /*=== sys_write ===*/
        64 => {
          //一個dw只有64bit，只能容納8個char需算出總共多少個dw
          var dw_count : int = 0;   //計算多少個double world 這裡為0的話 其實代表1個dw
          var char_count : int = 0; //計算寫了多少個char 當作write()的回傳值
          //如果字串長度為8的倍數，會導致dw_count每次算出來都多1，例如a2=8,dw_count=1，所以改成如果為8的倍數就先減1 
          if a2 > 0 & emod_int(a2, 8) == 0  
          then dw_count = ediv_int(sub_int(a2,1),8)
          else dw_count = ediv_int(a2,8);

          //利用先前算出來的dw_count把所有字串印出來
          foreach(i from 0 to dw_count){
              //如果a2 - (i*8)大於8，也就是說字串剩餘長度大於8，接下來要把一個dw給印出來
              var char_len : int = 0;
              if a2 - (i * 8) > 8  
              then char_len = 8
              else char_len = (a2 - (i * 8));
              
              //把字串讀出來，位置為a1_bits_var + (i * 8)
              let temp_tuple = read_ram(Read_plain, physaddr(a1_bits_var + (i * 8)) , 8, false);
              let (char_bits_const, _) = temp_tuple;
              //print("string = " ^ BitStr(char_bits_const));

              var char_bits_var : xlenbits = char_bits_const;
                foreach(j from 0 to sub_int(char_len,1)){
                  //fwrite()會將0印出，因此需要檢查char如果為0，就不要寫入
              //    if char_bits_var[7 .. 0] != 0x00
                //  then {
                    char_count = char_count + 1;
                    plat_term_write(a0_bits_var, char_bits_var[7 .. 0]);
                  //};
                  
                  //print("char = " ^ BitStr(char_bits_var[7 .. 0]));

                  char_bits_var = char_bits_var >> 8;
                };
                //讀完後需要把剛剛的記憶體給清0
                let _ = write_ram(Write_plain, physaddr(a1_bits_var + (i * 8)), 8,  zero_extend(0b0), ());
          };
          //print_int("char_count = " , char_count);
          if char_count == 0
          then {let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8,  sign_extend(0b1), ())}
          else {let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8,  zero_extend(bits_of_int(char_count)), ())};
        },
        /*=== sys_fstat === xxx*/
        80 => {
          let ret = plat_term_fstat(a0_bits_var);
          //print("st_ino = " ^ BitStr(st_ino));

          let _ = write_ram(Write_plain, physaddr(a1_bits_var) , 2, st_dev, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 2) , 2, st_ino, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 4) , 4, st_mode, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 8) , 2, st_nlink, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 10) , 2, st_uid, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 12) , 2, st_gid, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 14) , 2, st_rdev, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 16) , 8, st_s, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 24) , 8, st_atim_sec, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 32) , 8, st_atim_nsec, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 40) , 8, st_mtim_sec, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 48) , 8, st_mtim_nsec, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 56) , 8, st_ctim_sec, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 64) , 8, st_ctim_nsec, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 72) , 8, st_blk, ());
          let _ = write_ram(Write_plain, physaddr(a1_bits_var + 80) , 8, st_blocks, ());

            let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8,  ret, ());

        },
      /*=== sys_exit ===*/
        93 => {
            let x : xlenbits = to_bits(sizeof(xlen), 1);
            //print("to_bits" ^ BitStr(x));
            htif_done = true;
            htif_exit_code =  (a0_bits_var << 1) | x;

        },
        /*=== sys_times. ===*/
        168 => {
            let _ = plat_term_times();
            let _ = write_ram(Write_plain, physaddr(a0_bits_var) , 8, tms_utime, ());
            let _ = write_ram(Write_plain, physaddr(a0_bits_var + 8), 8, zero_extend(0b0), ());
            let _ = write_ram(Write_plain, physaddr(a0_bits_var + 16), 8, zero_extend(0b0), ());
            let _ = write_ram(Write_plain, physaddr(a0_bits_var + 24), 8, zero_extend(0b0), ());

            let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8,  tms_utime, ());

        },
        /*=== sys_utime ===*/
        1000 => {
            /*
            最大字元數先設為256個
            64bits為8個字元 所以256/8 = 32
            */
            
            var flag : bool = true;
            var i : int = 0;
            while flag & i<=31 do {
              let temp_tuple = read_ram(Read_plain, physaddr(a0_bits_var + (i * 8)) , 8, false);
              let (char_bits_const, _) = temp_tuple;
              //print("string = " ^ BitStr(char_bits_const));

              var char_bits_var : xlenbits = char_bits_const;
              foreach(j from 0 to 7){
                  if char_bits_var[7 .. 0] == 0x00
                  then flag = false
                  else {
                    getChar2C(char_bits_var[7 .. 0]);
                    char_bits_var = char_bits_var >> 8;
                  }
              };             
              i = i + 1;
            };

            let temp_tuple = read_ram(Read_plain, physaddr(a1_bits_var), 8, false);
            let (actime_bits, _) = temp_tuple;
            let temp_tuple2 = read_ram(Read_plain, physaddr(a1_bits_var + 8), 8, false);
            let (modtime_bits, _) = temp_tuple2;
            //print("string = " ^ BitStr(char_bits_const));

            let ret = plat_term_utime(actime_bits, modtime_bits);
            let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8,  ret, ());

        },
        /*=== sys_gettimeofday. ===*/
        169 => {
            let _ = plat_term_gettimeofday();
            let _ = write_ram(Write_plain, physaddr(a0_bits_var) , 8, tv_sec, ());
            let _ = write_ram(Write_plain, physaddr(a0_bits_var + 8) , 8, tv_usec, ());

            let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8,  zero_extend(0b0), ());

        },

        /*=== sys_open ===*/
        1024 => {

            /*
            最大字元數先設為256個
            64bits為8個字元 所以256/8 = 32
            */
            var flag : bool = true;
            var i : int = 0;
            while flag & i<=31 do {
              let temp_tuple = read_ram(Read_plain, physaddr(a0_bits_var + (i * 8)) , 8, false);
              let (char_bits_const, _) = temp_tuple;
              //print("string = " ^ BitStr(char_bits_const));

              var char_bits_var : xlenbits = char_bits_const;
              foreach(j from 0 to 7){
                  if char_bits_var[7 .. 0] == 0x00
                  then flag = false
                  else {
                    getChar2C(char_bits_var[7 .. 0]);
                    char_bits_var = char_bits_var >> 8;
                  }
              };             
              i = i + 1;
            };
            let _ = write_ram(Write_plain, physaddr(sail_zero_extend(cmd[payload], 64)), 8,  plat_term_open(a1, a2), ());

        },
        /*=== sys_getmainvars ===*/
        2011 => {
            
            var vec_argv : vector(10, dec, string) = ["0","0","0","0","0","0","0","0","0","0"];
            var vec_word : vector(10, dec, int) = [0,0,0,0,0,0,0,0,0,0];
            var vec_addr : vector(10, dec, int) = [0,0,0,0,0,0,0,0,0,0];

            let _ = write_ram(Write_plain, physaddr(a0_bits_var), 8, bits_of_int(htif_argc), ());
            //print_int("htif_argc=" , htif_argc);
            //print("htif_argc=" ^ BitStr(bits_of_int(htif_argc)));

/*
            let _ = write_ram(Write_plain, a0_bits_var + 8, 8, a0_bits_var + zero_extend(0x20), ());
            let _ = write_ram(Write_plain, a0_bits_var + 16, 8, zero_extend(0x0), ());
            let _ = write_ram(Write_plain, a0_bits_var + 24, 8, zero_extend(0x0), ());
            let _ = write_ram(Write_plain, a0_bits_var + 32, 8, zero_extend(0x3837363534333231), ());
            let _ = write_ram(Write_plain, a0_bits_var + 40, 8, zero_extend(0x3132333435363738), ());
*/

            //get argv string and len
            foreach(i from 0 to sub_int(htif_argc, 1)){
              if i < 10
              then { vec_argv[i] = get_argv(i);
                      //print("vec_argv=" ^ vec_argv[i]);
                      var vec_argv_count : int = 0;
                      if emod_int(string_length(vec_argv[i]) + 1, 8) == 0  
                      then vec_argv_count = ediv_int(sub_int(string_length(vec_argv[i]) + 1, 1), 8)
                      else vec_argv_count = ediv_int(string_length(vec_argv[i]) + 1, 8);
                      vec_word[i]  = vec_argv_count;
                      //print_int("vec_argv = " , string_length(vec_argv[i])+1 );
                      //print_int("vec_word=" , vec_word[i]);
              }
            };
            
            
            //set argv address 
            var count_word : int = htif_argc + 2 + 1;
            //print("=================================================");

            foreach(i from 0 to sub_int(htif_argc, 1)){
              if i < 10
              then { vec_addr[i] = count_word;
                      //print_int("vec_addr = ", vec_addr[i]);

                      let _ = write_ram(Write_plain, physaddr(a0_bits_var + (i+1)*8) , 8, a0_bits_var + bits_of_int(vec_addr[i]*8), ());
                      //print_int("a0_bits_var + " , (i+1)*8);
                      //print("a0_bits_var + " ^ BitStr(bits_of_int(vec_addr[i]*8)));

                      count_word = count_word + vec_word[i] + 1;
              }
            };  
            //print("=================================================");

            //set null 
            let _ = write_ram(Write_plain, physaddr(a0_bits_var + (htif_argc+1)*8) , 8, zero_extend(0x0), ());
            let _ = write_ram(Write_plain, physaddr(a0_bits_var + (htif_argc+2)*8) , 8, zero_extend(0x0), ());

            //set argv char


            foreach(i from 0 to sub_int(htif_argc, 1)){
              if i < 10
              then {
                
                var vec_argv_count : int = 0;
                if emod_int(string_length(vec_argv[i]) + 1, 8) == 0  
                then vec_argv_count = ediv_int(sub_int(string_length(vec_argv[i]) + 1, 1),8)
                else vec_argv_count = ediv_int(string_length(vec_argv[i]) + 1,8);
                //print_int("vec_argv_count = ", vec_argv_count);
                
                foreach(j from 0 to vec_argv_count /*ediv_int(string_length(vec_argv[i]) ,8)*/){
                  var append_str : bits(64) = zero_extend(0b0);
                  var len_copy : int = 0;
                  if string_length(vec_argv[i]) - (j * 8) > 8  
                  then len_copy = 8
                  else len_copy = (string_length(vec_argv[i]) - (j * 8));
                  foreach(k from 0 to (len_copy - 1)){
                    let bos = bits_of_string(vec_argv[i], k + j*8);
                    if k < 8
                    then append_str = vector_update_subrange(append_str, k*8+7 ,k*8, bos); 
                  //  print("append_str = " ^ BitStr(append_str));
                  };

                  let _ = write_ram(Write_plain, physaddr(a0_bits_var + (vec_addr[i] + j)*8), 8, append_str, ());

                };
              }
            };

        },
        n => { 
            ()
            //print_int("bad syscall #", n);
            //let x : xlenbits = to_bits(sizeof(xlen), 1);
            //htif_done = true;
            //htif_exit_code =  (a0_bits_var << 1) | x;
        }

    };
    
    /*=== if payload[0] eqaul 1, program stop ===*/
    
    if cmd[payload][0] == bitone
    then {
        /* cache */
        // print_ic_stats();
        // print_dc_stats();
        // print_l2_stats();
        /* cache */
        show_performance();
        show_cache_info();
        show_branch_pred_info();
        htif_done = true;
        //print("payload = " ^ BitStr(cmd[payload]));
        htif_exit_code = (sail_zero_extend(cmd[payload], 64) >> 1);
        //print("payload = " ^ BitStr(htif_exit_code));
    };
    
    /*======================================*/
    let a = plat_htif_fromhost();
    //print("fromhost = " ^ BitStr(a));
    reset_htif()
  };
  let _ = write_ram(Write_plain, physaddr(plat_htif_tohost()+8), 8, sign_extend(0b1), ());
  /* for sysycall */
  MemValue(true)
}

val htif_tick : unit -> unit
function htif_tick() = {
  if   get_config_print_platform()
  then print_platform("htif::tick " ^ BitStr(htif_tohost));
  htif_tohost = htif_tohost /* prevent this function being optimized out */
}

/* Top-level MMIO dispatch */
$ifndef RVFI_DII
function within_mmio_readable forall 'n, 0 < 'n <= max_mem_access . (addr : physaddr, width : int('n)) -> bool =
  within_clint(addr, width) | (within_htif_readable(addr, width) & 1 <= 'n)
$else
function within_mmio_readable forall 'n, 0 < 'n <= max_mem_access . (addr : physaddr, width : int('n)) -> bool = false
$endif

$ifndef RVFI_DII
function within_mmio_writable forall 'n, 0 < 'n <= max_mem_access . (addr : physaddr, width : int('n)) -> bool =
  within_clint(addr, width) | (within_htif_writable(addr, width) & 'n <= 8)
$else
function within_mmio_writable forall 'n, 0 < 'n <= max_mem_access . (addr : physaddr, width : int('n)) -> bool = false
$endif

function mmio_read forall 'n, 0 < 'n <= max_mem_access . (t : AccessType(ext_access_type), paddr : physaddr, width : int('n)) -> MemoryOpResult(bits(8 * 'n)) =
  if   within_clint(paddr, width)
  then clint_load(t, paddr, width)
  else if within_htif_readable(paddr, width) & (1 <= 'n)
  then htif_load(t, paddr, width)
  else match t {
    Execute()  => MemException(E_Fetch_Access_Fault()),
    Read(Data) => MemException(E_Load_Access_Fault()),
    _          => MemException(E_SAMO_Access_Fault())
  }

function mmio_write forall 'n, 0 <'n <= max_mem_access . (paddr : physaddr, width : int('n), data: bits(8 * 'n)) -> MemoryOpResult(bool) =
  if   within_clint(paddr, width)
  then clint_store(paddr, width, data)
  else if within_htif_writable(paddr, width) & 'n <= 8
  then htif_store(paddr, width, data)
  else MemException(E_SAMO_Access_Fault())

/* Platform initialization and ticking. */

function init_platform() -> unit = {
  htif_tohost = zeros();
  htif_done   = false;
  htif_exit_code = zeros();
  htif_cmd_write = bitzero;
  htif_payload_writes = zeros();
}

function tick_platform() -> unit = {
  htif_tick();
}

/* Platform-specific handling of instruction faults */

function handle_illegal() -> unit = {
  let info = if plat_mtval_has_illegal_inst_bits ()
             then Some(instbits)
             else None();
  let t : sync_exception = struct { trap    = E_Illegal_Instr(),
                                    excinfo = info,
                                    ext     = None() };
  set_next_pc(exception_handler(cur_privilege, CTL_TRAP(t), PC))
}

/* Platform-specific wait-for-interrupt */
function platform_wfi() -> unit = ()
