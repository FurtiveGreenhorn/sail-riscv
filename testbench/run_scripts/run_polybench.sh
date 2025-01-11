#!/bin/bash

SIMULATOR="../c_emulator/riscv_sim_RV64 -V -p"
POLYBENCH_DIR="polybench"
RESULT_DIR="polybench_results"

# 判斷資料夾是否存在，若不存在則創建
if [ ! -d "$RESULT_DIR" ]; then
  echo "資料夾 $RESULT_DIR 不存在，正在創建..."
  mkdir "$RESULT_DIR"
fi  # 關閉 if 區塊

# 進行 Polybench 測試
for testfile in "${POLYBENCH_DIR}/"*; do
  testfile_name=$(basename "$testfile")
  
  echo "running ${testfile_name}..."
  $SIMULATOR "${testfile}" > "${RESULT_DIR}/${testfile_name}.log" 2>&1
done
