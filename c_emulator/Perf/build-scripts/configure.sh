#!/bin/bash

# --- 設定變數 ---
# 獲取腳本檔案所在的目錄 (例如 Perf/build-scripts/)
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# 專案根目錄是腳本目錄的父目錄
PROJECT_ROOT=$(dirname "$SCRIPT_DIR")

# 建構產物的輸出目錄名稱
BUILD_DIR_NAME="build" # 這裡只是名稱，不是完整路徑

# --- 顏色輸出 ---
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}--- 開始配置專案 ---${NC}"

# 確保在專案根目錄下進行操作
echo -e "${YELLOW}切換到專案根目錄: ${PROJECT_ROOT}${NC}"
cd "$PROJECT_ROOT" || { echo -e "${RED}錯誤: 無法切換到專案根目錄 ${PROJECT_ROOT}${NC}"; exit 1; }

# 進入或建立建構目錄 (現在相對於 PROJECT_ROOT)
echo -e "${YELLOW}進入或建立建構目錄: ${BUILD_DIR_NAME}${NC}"
mkdir -p "$BUILD_DIR_NAME" || { echo -e "${RED}錯誤: 無法建立建構目錄 ${BUILD_DIR_NAME}${NC}"; exit 1; }
cd "$BUILD_DIR_NAME" || { echo -e "${RED}錯誤: 無法進入建構目錄 ${BUILD_DIR_NAME}${NC}"; exit 1; }

# 執行 CMake 配置
BUILD_TESTING_FLAG="-DBUILD_TESTING=ON"
if [[ "$1" == "--no-tests" ]]; then
    BUILD_TESTING_FLAG="-DBUILD_TESTING=OFF"
    echo -e "${YELLOW}注意: 測試建構已關閉.${NC}"
fi

echo -e "${GREEN}執行 CMake 配置...${NC}"
# 注意: 因為我們已經切換到 build 目錄，所以源碼目錄使用 .. (回到專案根目錄)
cmake "${BUILD_TESTING_FLAG}" .. || { echo -e "${RED}錯誤: CMake 配置失敗${NC}"; exit 1; }

echo -e "${GREEN}--- 配置完成 ---${NC}"
