# 編譯器設定
CXX       := g++
CXXFLAGS  := -O2 -Wall -I./src
MAKELIB = ar crs $@

# 原始碼與物件檔目錄設定
SRC_DIR   := src
BUILD_DIR := build
SAIL_DIR := $(shell sail --dir)
SAIL_LIB_DIR := $(SAIL_DIR)/lib

# 自動尋找所有的 .cpp 檔 (包含子目錄)
SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp")

# 將 src 中的路徑轉換為 build 中對應的 .o 檔案路徑
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# 靜態函式庫輸出檔名
LIB := $(BUILD_DIR)/cachelib.a

.PHONY: all clean

all: $(LIB)

# 產生靜態庫
$(LIB): $(OBJS)
	$(MAKELIB) $^

# 編譯規則：將 .cpp 編譯為 .o
# FixMe: 無法根據依賴自動更新，當前檔案更動後需要make clean後才能重新make
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)   # 自動建立目錄
	$(CXX) -g $(CXXFLAGS) -c -I$(SAIL_LIB_DIR) $< -o $@

clean:
	rm -rf $(BUILD_DIR)
