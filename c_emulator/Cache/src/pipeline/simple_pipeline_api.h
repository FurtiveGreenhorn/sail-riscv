// 此頭文件的目的:
// 1.只提供 sail model 需要使用的函式
// 2.轉換(參數類型、回傳值)成sail可接受的定義格式

#pragma once
#include "sail.h"

#ifdef __cplusplus
extern "C" {
#endif

unit show_instructions(mach_bits rs1, mach_bits rs2, mach_bits rd);

#ifdef __cplusplus
}
#endif