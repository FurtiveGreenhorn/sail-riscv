#!/bin/bash

# 設定來源目錄與結果輸出目錄
INPUT_DIR="rv64im-test/polyim_output"
OUTPUT_DIR="result"
SIMULATOR="c_emulator/riscv_sim_RV64"

# 檢查輸出目錄是否存在，若不存在則建立
if [ ! -d "$OUTPUT_DIR" ]; then
    echo "創建結果目錄: $OUTPUT_DIR"
    mkdir -p "$OUTPUT_DIR"
fi

# 檢查模擬器檔案是否存在
if [ ! -f "$SIMULATOR" ]; then
    echo "模擬器不存在: $SIMULATOR"
    exit 1
fi

# 逐一處理 INPUT_DIR 目錄下的每個檔案
for file in "$INPUT_DIR"/*; do
    # 確保是檔案而非目錄
    if [ -f "$file" ]; then
        # 取得檔案名稱（不含路徑）
        filename=$(basename "$file")
        # 定義輸出結果檔名
        result_file="$OUTPUT_DIR/${filename}_result"

        # 執行模擬器並將輸出重定向到結果檔案
        echo "執行: $filename"
        $SIMULATOR -V "$file" > "$result_file"

        # 顯示完成訊息
        echo "已完成: $result_file"
    fi
done

echo "所有檔案處理完成，結果存放於 $OUTPUT_DIR"
