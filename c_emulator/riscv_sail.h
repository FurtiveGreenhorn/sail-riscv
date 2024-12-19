/* Top-level interfaces to the Sail model.
   Ideally, this would be autogenerated.
 */

typedef int unit;
#define UNIT 0
typedef uint64_t mach_bits;

struct zMisa {
  mach_bits zMisa_chunk_0;
};
extern struct zMisa zmisa;

void model_init(void);
void model_fini(void);

unit zinit_model(unit);
bool zstep(sail_int);
unit ztick_clock(unit);
unit ztick_platform(unit);

#ifdef RVFI_DII
unit zext_rvfi_init(unit);
unit zrvfi_set_instr_packet(mach_bits);
mach_bits zrvfi_get_cmd(unit);
mach_bits zrvfi_get_insn(unit);
bool zrvfi_step(sail_int);
unit zrvfi_zzero_exec_packet(unit);
unit zrvfi_halt_exec_packet(unit);
void zrvfi_get_exec_packet_v1(sail_bits *rop, unit);
void zrvfi_get_exec_packet_v2(sail_bits *rop, unit);
extern bool zrvfi_int_data_present;
void zrvfi_get_int_data(sail_bits *rop, unit);
extern bool zrvfi_mem_data_present;
void zrvfi_get_mem_data(sail_bits *rop, unit);
mach_bits zrvfi_get_v2_trace_sizze(unit);
void zrvfi_get_v2_support_packet(sail_bits *rop, unit);

// Debugging prints
unit zprint_rvfi_exec(unit);
unit zprint_instr_packet(uint64_t);
#endif

extern mach_bits zxlen_val;
extern bool zhtif_done;
extern mach_bits zhtif_exit_code;
extern bool have_exception;

/* machine state */

extern uint32_t zcur_privilege;

extern mach_bits zPC;

extern mach_bits zx1, zx2, zx3, zx4, zx5, zx6, zx7, zx8, zx9, zx10, zx11, zx12,
    zx13, zx14, zx15, zx16, zx17, zx18, zx19, zx20, zx21, zx22, zx23, zx24,
    zx25, zx26, zx27, zx28, zx29, zx30, zx31;

extern mach_bits zmstatus;
extern mach_bits zmepc, zmtval;
extern mach_bits zsepc, zstval;

extern mach_bits zfloat_result, zfloat_fflags;

struct zMcause {
  mach_bits zMcause_chunk_0;
};
extern struct zMcause zmcause, zscause;

extern mach_bits zminstret;
