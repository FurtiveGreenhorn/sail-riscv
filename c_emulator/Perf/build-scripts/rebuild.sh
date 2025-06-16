#!/bin/bash

# --- 設定變數 ---
# 獲取腳本檔案所在的目錄 (例如 Perf/build-scripts/)
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# 專案根目錄是腳本目錄的父目錄 (例如 Perf/)
PROJECT_ROOT=$(dirname "$SCRIPT_DIR")

# 建構產物的輸出目錄名稱
BUILD_DIR_NAME="build"

# --- 顏色輸出 ---
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${GREEN}--- 開始完整重建專案 ---${NC}"

# 確保在專案根目錄下進行操作
echo -e "${YELLOW}切換到專案根目錄: ${PROJECT_ROOT}${NC}"
cd "$PROJECT_ROOT" || { echo -e "${RED}錯誤: 無法切換到專案根目錄 ${PROJECT_ROOT}${NC}"; exit 1; }

# 1. 刪除舊的建構目錄 (執行 rm -rf build/)
echo -e "${YELLOW}正在清理舊的建構目錄 '${BUILD_DIR_NAME}'...${NC}"
rm -rf "$BUILD_DIR_NAME" || { echo -e "${RED}錯誤: 無法刪除舊的建構目錄 ${BUILD_DIR_NAME}${NC}"; exit 1; }
echo -e "${GREEN}舊的建構目錄已清理。${NC}"

# 2. 運行配置腳本
echo -e "\n${BLUE}--- 執行配置腳本 (${SCRIPT_DIR}/configure.sh) ---${NC}"
# 將任何傳給 rebuild.sh 的參數傳遞給 configure.sh (例如 --no-tests)
"${SCRIPT_DIR}/configure.sh" "$@" || { echo -e "${RED}錯誤: 配置腳本執行失敗${NC}"; exit 1; }

# 3. 運行建構腳本
echo -e "\n${BLUE}--- 執行建構腳本 (${SCRIPT_DIR}/build.sh) ---${NC}"
# 將任何傳給 rebuild.sh 的參數傳遞給 build.sh (例如 -j8)
"${SCRIPT_DIR}/build.sh" "$@" || { echo -e "${RED}錯誤: 建構腳本執行失敗${NC}"; exit 1; }

echo -e "\n${GREEN}--- 專案已成功完整重建 ---${NC}"
