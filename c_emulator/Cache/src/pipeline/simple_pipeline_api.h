// 此頭文件的目的:
// 1.只提供 sail model 需要使用的函式
// 2.轉換(參數類型、回傳值)成sail可接受的定義格式

#pragma once
#include "sail.h"

#ifdef __cplusplus
extern "C" {
#endif

unit createInstrForStageInfo();
unit sendInstToPipeline();
unit show_cycle_count();
unit read_addr(uint64_t addr);
unit read_Rtype(mach_bits rs1, mach_bits rs2, mach_bits rd);
unit read_Itype(mach_bits rs1, mach_bits rd);
unit read_Itype_xRs1(mach_bits rd);
unit read_Itype_xRs1xRd();
unit read_SBtype(mach_bits rs1, mach_bits rs2);
unit read_UJtype(mach_bits rd);
unit read_type(const int type);
// unit read_rs(mach_bits rs1, mach_bits rs2);
// unit read_rs1(mach_bits rs1);
// unit read_rd(mach_bits rd);
unit read_taken(bool taken);

#ifdef __cplusplus
}
#endif