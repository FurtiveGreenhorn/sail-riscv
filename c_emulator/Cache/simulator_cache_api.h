// 此頭文件的目的:
// sail simulator 產生 riscv_model_RV64 的方式只接受c格式:

// generated_definitions/c/riscv_model_$(ARCH).c: $(SAIL_SRCS) model/main.sail Makefile
// 	mkdir -p generated_definitions/c
// 	$(SAIL) $(SAIL_FLAGS) $(c_preserve_fns) -O -Oconstant_fold -memo_z3 -c \
//  c_include riscv_prelude.h -c_include riscv_platform.h -c_include \
//  Cache/simulator_cache_api.h -c_no_main $(SAIL_SRCS) model/main.sail -o $(basename $@)


// 原本的api必須先由c++編譯成c格式無法被作為c文件處理
// 因此額外添加此層來提供給

#pragma once
#include "src/main_cache/riscv_cpp_test.h"
#include "src/Instruction/instruction_api.h"