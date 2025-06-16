#!/bin/bash

# --- 設定變數 ---
# 獲取腳本檔案所在的目錄 (例如 Perf/build-scripts/)
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# 專案根目錄是腳本目錄的父目錄
PROJECT_ROOT=$(dirname "$SCRIPT_DIR")

# 建構產物的輸出目錄名稱
BUILD_DIR_NAME="build"

# --- 顏色輸出 ---
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}--- 開始建構專案 ---${NC}"

# 確保在專案根目錄下進行操作
echo -e "${YELLOW}切換到專案根目錄: ${PROJECT_ROOT}${NC}"
cd "$PROJECT_ROOT" || { echo -e "${RED}錯誤: 無法切換到專案根目錄 ${PROJECT_ROOT}${NC}"; exit 1; }

# 進入建構目錄
if [ ! -d "$BUILD_DIR_NAME" ]; then
    echo -e "${RED}錯誤: 建構目錄 '${BUILD_DIR_NAME}' 不存在。請先執行 'configure.sh'。${NC}"
    exit 1
fi
echo -e "${YELLOW}進入建構目錄: ${BUILD_DIR_NAME}${NC}"
cd "$BUILD_DIR_NAME" || { echo -e "${RED}錯誤: 無法進入建構目錄 ${BUILD_DIR_NAME}${NC}"; exit 1; }

# 執行 Make 編譯 (或 clean 等目標)
echo -e "${GREEN}執行 Make 命令...${NC}"
make "$@" || { echo -e "${RED}錯誤: Make 命令執行失敗${NC}"; exit 1; }

echo -e "${GREEN}--- 建構/命令完成 ---${NC}"
