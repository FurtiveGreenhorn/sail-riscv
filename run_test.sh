#!/bin/bash

# 預設測試檔案
DEFAULT_TEST="testbench/polybench/trmm"

# 如果有提供參數，使用參數作為測試檔案；否則使用預設檔案
TEST_FILE=${1:-$DEFAULT_TEST}

# 執行模擬器
echo "c_emulator/riscv_sim_RV64 with test file: $TEST_FILE"
c_emulator/riscv_sim_RV64 -V "$TEST_FILE"

# 確認執行結果
if [ $? -eq 0 ]; then
    echo "Execution completed successfully."
else
    echo "Execution failed. Please check the error messages above."
fi
