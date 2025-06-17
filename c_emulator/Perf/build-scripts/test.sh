#!/bin/bash

# --- 設定變數 ---
# 獲取腳本檔案所在的目錄 (例如 Perf/build-scripts/)
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# 專案根目錄是腳本目錄的父目錄
PROJECT_ROOT=$(dirname "$SCRIPT_DIR")

# 建構產物的輸出目錄名稱
BUILD_DIR_NAME="build"

# CTest 預設參數
CTEST_ARGS="--output-on-failure" # 預設只在失敗時輸出

# --- 顏色輸出 ---
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m' # Added YELLOW for consistency
NC='\033[0m'

# --- 處理命令行參數 ---
for arg in "$@"; do
    case "$arg" in
        -v|--verbose)
            CTEST_ARGS="-V" # 如果有 -v 或 --verbose 旗標，則使用 -V
            echo -e "${YELLOW}偵測到 -v/--verbose 旗標，將啟用詳細測試輸出。${NC}"
            ;;
        *)
            # 其他未知參數可以傳遞給 CTest，但這裡我們只處理 -v
            # 如果你希望傳遞其他 CTest 參數，"$@" 已經處理了，所以不需要額外在這裡捕獲
            ;;
    esac
done

echo -e "${GREEN}--- 開始運行測試 ---${NC}"

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

# 執行 CTest 測試
echo -e "${GREEN}執行 CTest 測試...${NC}"
# 將 $CTEST_ARGS 變數傳遞給 ctest
ctest $CTEST_ARGS "$@" || { echo -e "${RED}錯誤: 測試失敗${NC}"; exit 1; }

echo -e "${GREEN}--- 測試運行完成 ---${NC}"