#pragma once

// 確保 gmp.h 在 sail.h 之前被包含，因為 sail.h 依賴 mpz_t。
// gmp.h 是一個 C 語言頭文件，它自己會處理 C/C++ 的兼容性 (internally extern "C" guarded)。
// 所以它不需要被放在 extern "C" 區塊內。
#include <gmp.h> // <--- 確保這一行在這裡，位於任何 extern "C" 之前，和 sail.h 之前

#ifdef __cplusplus
extern "C" {
#endif

// 將 sail.h 放在 extern "C" 區塊內，以確保 C++ 編譯器正確處理 C 風格的函式連結。
#include "sail.h" // sail.h 需要 mpz_t，現在 gmp.h 已經包含進來了

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

unit createInstrForStageInfo();
unit sendInstToPipeline();
unit show_performance();
unit show_cache_info();

unit read_addr(uint64_t addr);
unit read_ls_addr(uint64_t addr);

unit read_rs_val(uint64_t rs1_val, uint64_t rs2_val);
// unit read_rs1_val(uint64_t rs1_val);
// unit read_rs2_val(uint64_t rs2_val);

unit read_type(const int type);

unit read_Rtype_regs(mach_bits rs2, mach_bits rs1, mach_bits rd);
unit read_Itype_regs(mach_bits rs1, mach_bits rd);

unit read_Itype_xRs1(mach_bits rd);
unit read_Itype_xRs1xRd();
unit read_SBtype_regs(mach_bits rs2, mach_bits rs1);
unit read_UJtype_regs(mach_bits rd);

unit read_taken(bool taken);

// unit read_rs(mach_bits rs1, mach_bits rs2);
// unit read_rs1(mach_bits rs1);
// unit read_rd(mach_bits rd);

#ifdef __cplusplus
}
#endif