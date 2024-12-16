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

unit set_rs(mach_bits rs1, mach_bits rs2);

unit set_rs1(mach_bits rs1);

unit set_rs2(mach_bits rs2);

unit set_rd(mach_bits rd);

unit set_taken(bool taken);

#ifdef __cplusplus
}
#endif