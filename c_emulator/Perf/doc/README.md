# Perf - 模組化微架構 CPU 模擬器效能模型

這個 `Perf` 專案旨在建立一個**高度模組化且可組裝的微架構 CPU 模擬器效能模型**。其核心目標是提供一個靈活的框架，讓使用者能夠自由選用、組裝、擴充或替換各種 CPU 部件（如流水線、快取、分支預測單元、執行單元等），以快速定義不同的微架構配置，並與任意功能模擬器結合以獲取詳細的效能分析資訊。

**目前，本專案作為 `sail-riscv` 專案的一個核心組件，與其高度耦合。** 它的編譯、連結和執行都緊密依賴於 `sail-riscv` 的建構系統。**本效能模型通過 C 語言介面 (`perf_model.h`)，將其 API 呼叫直接嵌入到 Sail 模型本身的 `.sail` 描述檔案中。** 這樣，Sail 生成 C 模擬器時，就能在關鍵的指令執行點位自動觸發 `Perf` 的邏輯，實現精細的微架構事件追蹤。

本專案已成功與 **Sail 功能模擬器**結合，並已開始設計核心的**時鐘（Clock）機制**作為第一個基礎構件。

---

## ✨ 專案核心願景

* **積木式組裝：** 提供一系列微架構組件（例如 `Pipeline Stage`, `Cache`, `BPU`, `LSUnit`, `Execution Unit` 等），讓使用者能夠像積木一樣選用並組裝自己的 CPU 核心模型。
* **高度靈活可擴展：**
    * 每個組件可以有**簡單或進階的版本**，根據需求進行替換。
    * 可以**跳過**不需要的組件，減少模擬的變數，專注於特定單元的效能分析。
    * 支援對現有組件進行**功能擴充**，而非僅限於選用。
* **功能模擬器整合：** 旨在與**任意功能模擬器結合**（目前已與 **Sail** 成功整合），充當其微架架構層的效能資訊提供者。
* **細緻的效能追蹤：** 能夠產出詳細的微架構執行數據，為 CPU 設計和優化提供洞察。

---

## 🚀 主要特性

* **`sail-riscv` 深度整合：**
    * **在 Sail 描述中嵌入 API 呼叫：** 透過直接在 `.sail` 檔案中插入 `Perf` 的 C 語言 API 呼叫，實現微架構事件的精確捕捉。
    * **Makefile 編譯整合：** 透過修改 `sail-riscv` 的 Makefile 實現無縫建構，將 `Perf` 模型作為靜態函式庫連結到 Sail 生成的 C 模擬器中。
* **C 語言介面 API：** 提供一套符合 C 語言慣例的介面 (`perf_model.h`)，方便 Sail 模型在執行關鍵指令時呼叫，傳遞微架構事件資訊。
* **模組化設計：** 為各個微架構組件（如時鐘、核心、指令單元）提供獨立且清晰的介面。
* **C++ 編譯期介面檢查：** 透過進階模板元編程技術（如 SFINAE），在編譯時期驗證型別是否滿足所需介面（例如時鐘概念），確保設計的穩健性。
* **現代化建構系統：** 每個 `Perf` 內部組件使用 **CMake** 進行建構管理。
* **單元測試：** 整合 **Google Test** 框架，提供結構化、可擴展的單元測試，確保每個組件的正確性。
* **自動化腳本：** `Perf` 內部提供一系列客製化 Bash 腳本，簡化其自身的建構、配置、測試與清理流程。

---

## 🛠️ 事前準備 (Prerequisites)

在建構和運行本專案之前，請確保您的系統安裝了以下工具：

* **C++ 相容編譯器：**
    * 支援您專案使用的 C++ 標準（例如 C++11, C++14, C++17 或更高版本）。
    * 例如：GCC, Clang, MSVC。
* **CMake：** 3.15 或更高版本
* **Google Test 開發函式庫：**
    * 通常可以透過您的套件管理器安裝。例如，在基於 Debian/Ubuntu 的系統上：
        ```bash
        sudo apt update
        sudo apt install libgtest-dev cmake
        # 接著手動編譯 Google Test 函式庫以生成 .a 檔案：
        sudo apt install libgmock-dev # 如果您也計畫使用 Google Mock (推薦但非必須)
        cd /usr/src/gtest
        sudo cmake .
        sudo make
        cd /usr/src/gmock # 如果您安裝了 libgmock-dev
        sudo cmake .
        sudo make
        ```
* **Sail-RISC-V 功能模擬器：**
    * 請確保 `sail-riscv` 專案已正確複製且其建構環境已配置。本專案將作為其內部的一個子模組。
    * `sail --dir` 命令應能返回其安裝路徑。
* **GMP (GNU Multiple Precision Arithmetic Library)：**
    * 由於 Sail 依賴 GMP，您的專案也將需要 GMP 的頭文件和函式庫。
    * 通常透過套件管理器安裝，例如在 Debian/Ubuntu 上：
        ```bash
        sudo apt install libgmp-dev
        ```
        **注意：** 儘管 Sail 可能有其自身 Opam 安裝的 GMP 版本，但為簡化本 C++ 專案的建構，我們當前配置為**直接連結系統安裝的 GMP**。請留意潛在的版本或 ABI 不一致風險，若遇問題，這將是排查重點。
* **SoftFloat-3e：**
    * 本專案依賴 `SoftFloat-3e` 函式庫。您需要確保 `SoftFloat-3e` 的程式碼位於 `c_emulator/SoftFloat-3e/` 路徑下，並且已根據 `sail-riscv` 的 Makefile 配置正確建構。

---

## 🚀 快速開始 (整合 `sail-riscv`)

由於 `Perf` 專案目前與 `sail-riscv` 高度耦合，其建構過程是作為 `sail-riscv` 整個模擬器建構的一部分。

### 1. 複製 `sail-riscv` 專案並放置 `Perf`

首先，請複製 `sail-riscv` 專案。然後，將您的 `Perf` 專案放置在 `sail-riscv/c_emulator/` 目錄下，使其路徑結構如下：

```
# 假設這是 sail-riscv 專案的根目錄
sail-riscv/
├── c_emulator/
│   ├── Perf/  <-- 您的 Perf 專案應放在這裡
│   │   ├── CMakeLists.txt
│   │   ├── build-scripts/
│   │   ├── model_interface/
│   │   └── ... (Perf 的其他文件)
│   ├── SoftFloat-3e/ <-- SoftFloat 函式庫
│   └── ... (sail-riscv c_emulator 的其他文件)
└── ... (sail-riscv 的其他文件和 Makefile)
```

### 2. 修改 `sail-riscv` 的 Makefile

要將 `Perf` 整合到 `sail-riscv` 的建構中，您需要修改 `sail-riscv` 根目錄下的 `Makefile`。找到 `c_emulator/riscv_sim_$(ARCH)` 目標，並加入 `Perf` 函式庫的建構和連結。

請在 `Makefile` 中添加或修改以下片段：

```makefile
# ... (Makefile 前面部分保持不變) ...

# <--- 在 Makefile 中找到合適的位置，通常在 C_LIBS / C_FLAGS 定義附近添加以下內容 --->

# 產生效能模型接口庫
PERF_DIR = c_emulator/Perf
PERF_LIBDIR = $(PERF_DIR)/build
PERF_LIBS = $(PERF_LIBDIR)/perf_model.a

$(PERF_LIBS): $(shell find $(PERF_DIR)/model_interface -type f) $(shell find $(PERF_DIR)/src -type f)
	# 確保 perf_model.a 被建構
	cd $(PERF_DIR) && mkdir -p build
	cd $(PERF_DIR)/build && cmake .. -DCMAKE_BUILD_TYPE=Release
	$(MAKE) -C $(PERF_DIR)/build perf_model

# ... (Makefile 中間部分保持不變) ...

# 將 Perf 的頭文件添加到 Sail C 代碼生成器的包含路徑中
# 找到 generated_definitions/c/riscv_model_$(ARCH).c 目標，添加 `-c_include Perf/model_interface/perf_model.h`
generated_definitions/c/riscv_model_$(ARCH).c: $(SAIL_SRCS) model/main.sail Makefile
	mkdir -p generated_definitions/c
	$(SAIL) $(SAIL_FLAGS) -O -Oconstant_fold -memo_z3 -c -c_include riscv_prelude.h -c_include riscv_platform.h -c_include Perf/model_interface/perf_model.h -c_no_main $(SAIL_SRCS) model/main.sail -o $(basename $@)

# ... (Makefile 中間部分保持不變) ...

# 修改 c_emulator/riscv_sim_$(ARCH) 目標，將 PERF_LIBS 加入連結
# 請確保 g++ 作為連結器，以便正確處理 C++ 程式碼
c_emulator/riscv_sim_$(ARCH): $(SAIL_OBJS_DIR)/riscv_model_$(ARCH).o $(PERF_LIBS) $(SAIL_OBJS) $(EMULATOR_OBJS) $(SOFTFLOAT_LIBS)
	g++ -g $^ $(C_LIBS_WRAPPED) -o $@

# ... (Makefile 其他部分保持不變) ...

# 在 clean 目標中，添加清理 Perf build 目錄的命令
clean:
	# ... (其他清理命令) ...
	-rm -rf c_emulator/Perf/build # <--- 添加這一行
```

### 3. 修改 `sail-riscv` 的 `.sail` 描述檔案

這是**最關鍵**的一步，將 `Perf` 效能模型與 Sail 功能模型連接起來。您需要修改 `sail-riscv` 專案中相關的 `.sail` 描述檔案，在指令執行的適當時間點插入 `Perf` 模型介面 (`perf_model.h`) 中定義的 API 呼叫。

例如，在處理指令解碼、執行、內存訪問、寫回等階段的 Sail 描述中：

* **引入外部 C 函式：** 在 `.sail` 檔案的頂部，使用 `external` 關鍵字引入 `perf_model.h` 中定義的 C 函式。例如：
    ```sail
    // 在 Sail 模型的 .sail 檔案中
    external unit read_addr(int64); // 引入 read_addr 函式
    external unit read_rs_val(int64, int64); // 引入 read_rs_val 函式
    // ... 引入其他您想從 Sail 呼叫的 Perf API ...
    ```
* **在執行邏輯中插入呼叫：** 在 Sail 程式碼中，當您模擬到特定事件時，呼叫這些外部函式。例如：
    * **指令解碼/執行後：** 在指令執行完成後，呼叫 `createInstrForStageInfo()`，或傳遞指令相關資訊。
    * **寄存器讀取：** 當讀取源寄存器值時，呼叫 `read_rs_val(rs1_val, rs2_val)`。
    * **內存訪問：** 當進行載入/儲存操作時，呼叫 `read_addr(mem_address)` 或 `read_ls_addr(address)`。
    * **分支預測：** 當判斷分支是否被跳轉時，呼叫 `read_taken(is_taken)`。

    這將需要在 `sail-riscv/model/` 目錄下的相關 `.sail` 檔案中進行具體修改。例如，在 `riscv_step.sail` 或 `riscv_mem.sail` 等文件中，根據指令的執行流程插入呼叫。

    **一個概念性的範例（在 Sail 偽程式碼中）：**
    ```sail
    function execute_instruction(i: instruction): unit = {
        let (rs1_val, rs2_val) = read_registers(i); // 假設從寄存器文件讀取
        // 呼叫 Perf API 傳遞寄存器值
        read_rs_val(rs1_val, rs2_val);

        match i.opcode {
            ADD => {
                let rd_val = rs1_val + rs2_val;
                write_register(i.rd, rd_val);
                // 呼叫 Perf API 傳遞寫回寄存器資訊
                read_Rtype_regs(rs2_val, rs1_val, i.rd);
            },
            LW => {
                let addr = calculate_load_address(i);
                // 呼叫 Perf API 傳遞內存訪問地址
                read_addr(addr);
                let loaded_val = read_memory(addr);
                write_register(i.rd, loaded_val);
            },
            JAL => {
                let target_addr = calculate_jump_target(i);
                let taken_branch = true; // 假設總是跳轉
                // 呼叫 Perf API 傳遞分支是否被跳轉
                read_taken(taken_branch);
                // ...
            }
            // ... 其他指令
        }
        // 在指令執行末尾，呼叫通用的指令完成 API
        sendInstToPipeline();
    }
    ```

### 4. 建構整合後的模擬器

在 `sail-riscv` 專案的根目錄下，運行 `make` 命令來建構整個模擬器（包括修改後的 Sail 模型和 `Perf` 模組）：

```bash
cd /path/to/sail-riscv/
make
```

這將會重新編譯 Sail 模型（將您的 API 呼叫轉譯為 C 程式碼）、編譯您的 `Perf` 模組，並將它們連結成最終的模擬器可執行檔 `c_emulator/riscv_sim_$(ARCH)`。

### 5. 運行 Perf 內部測試 (獨立於 Sail)

如果您想獨立地運行 `Perf` 模組的單元測試，您可以進入 `Perf` 自己的目錄並使用其 `build-scripts`：

```bash
cd /path/to/sail-riscv/c_emulator/Perf/
./build-scripts/test.sh
```

---

## 📂 專案結構與未來組件規劃

```
.
# 這是 Sail-RISC-V 專案的根目錄
├── CMakeLists.txt              # Sail-RISC-V 的主 CMake 配置檔案 (如果有的話)
├── Makefile                    # Sail-RISC-V 的主建構檔案 (需修改此文件以整合 Perf)
├── build-scripts/              # Sail-RISC-V 的自動化建構、配置和測試腳本
│   └── ...
├── c_emulator/                 # Sail-RISC-V 的 C 語言模擬器部分
│   ├── Perf/                   # <--- 您的 Perf 專案目錄
│   │   ├── CMakeLists.txt      # Perf 內部的主 CMake 配置檔案
│   │   ├── build-scripts/      # Perf 自己的自動化腳本
│   │   │   |-- build.sh
│   │   │   |-- configure.sh
│   │   │   |-- rebuild.sh
│   │   │   `-- test.sh
│   │   ├── model_interface/    # 效能模型對外介面層，與功能模擬器互動的核心
│   │   │   |-- CMakeLists.txt  # model_interface 的 CMake 配置
│   │   │   |-- perf_model.cpp  # 主要介面實作
│   │   │   `-- perf_model.h    # 介面定義 (供 Sail C 代碼呼叫)
│   │   ├── src/                # 微架構組件的核心實作
│   │   │   |-- Clock/          # 時鐘單元
│   │   │   |   `-- ClockConcept.h # 時鐘概念定義
│   │   │   |-- Core/           # 核心通用定義
│   │   │   |   `-- SimpleCore.h
│   │   │   `-- Instruction/    # 指令處理相關
│   │   │       |-- instruction.h
│   │   │       `-- instruction_pool.h
│   │   └── test/               # 所有單元測試程式碼
│   │       |-- CMakeLists.txt
│   │       `-- Clock/          # 時鐘組件的測試
│   │           `-- clock_concept_test.cpp
│   ├── SoftFloat-3e/           # <--- SoftFloat 函式庫
│   │   └── ... (SoftFloat 內部結構)
│   └── ... (其他 Sail C 模擬器文件)
├── model/                      # <--- Sail 模型的核心描述文件 (.sail 檔案)
│   ├── main.sail               # Sail 主入口
│   ├── prelude.sail            # 前置定義
│   ├── riscv_step.sail         # 指令步進邏輯 (可能需要插入 API)
│   ├── riscv_mem.sail          # 內存訪問邏輯 (可能需要插入 API)
│   └── ... (其他 .sail 描述檔案)
└── ... (Sail-RISC-V 的其他文件和工具)
```

### 未來組件分類規劃

根據專案高度模組化的目標，未來的微架構組件將會根據其職能進行分類，位於 `Perf/src/` 目錄下，例如：

* **`Perf/src/Pipeline/`**: 流水線級 (Pipeline Stage) 和 流水線暫存器 (Pipeline Register)。
* **`Perf/src/Cache/`**: 各級快取（L1, L2, L3 Cache）以及快取控制器 (Cache Controller)。
* **`Perf/src/BPU/`**: 分支預測單元 (Branch Prediction Unit)。
* **`Perf/src/LSUnit/`**: 載入/儲存單元 (Load/Store Unit)。
* **`Perf/src/ExecutionUnit/`**: 執行單元 (Execution Unit)，可能包含 ALU、乘除法器等。
* **`Perf/src/VectorUnit/`**: 向量處理單元 (Vector Unit)。
* **`Perf/src/Interconnect/`**: 互連單元 (Interconnect)，用於組件間通訊。
* ...以及其他未來可能加入的微架構組件。

每個組件目錄內將包含其對應的 `.h` 和 `.cpp` 檔案。這種結構將有助於實現**高度的靈活性**，允許使用者：

* **簡單/進階版本替換：** 每個組件都可以設計多個版本，通過 CMake 選項或條件編譯來選擇。
* **選擇性包含：** 根據模擬需求，只編譯和連結需要的組件，減少模擬變量和複雜度。
* **功能擴充：** 輕鬆在現有組件的基礎上擴充功能，而非從零開始。

---

## 💡 使用方式 (與 Sail-RISC-V 結合)

`Perf` 專案編譯為一個靜態函式庫 `perf_model.a`，它透過 `Perf/model_interface/perf_model.h` 中定義的 C 語言 API 暴露功能。**Sail-RISC-V 模擬器在執行指令的過程中，會直接從其 `.sail` 描述文件中調用這些 C 函數**，將微架構相關的事件和數據傳遞給 `Perf` 模組進行處理和統計。

### Sail `.sail` 檔案中的 API 呼叫範例：

1.  **在 `.sail` 檔案頂部引入外部 C 函式：**
    例如，在 `riscv_step.sail` 或其他相關 `.sail` 檔案的開頭：
    ```sail
    (* 將 perf_model.h 中的 C 函式宣告為 Sail 中的外部函式 *)
    external unit createInstrForStageInfo()
    external unit sendInstToPipeline()
    external unit read_addr(int64)
    external unit read_rs_val(int64, int64)
    external unit read_type(int)
    external unit read_Rtype_regs(bits(5), bits(5), bits(5)) (* 注意 Sail 的類型轉換 *)
    external unit read_Itype_regs(bits(5), bits(5))
    external unit read_Itype_xRs1(bits(5))
    external unit read_Itype_xRs1xRd()
    external unit read_SBtype_regs(bits(5), bits(5))
    external unit read_UJtype_regs(bits(5))
    external unit read_taken(bool)
    // ... 依據 perf_model.h 中的定義引入所有需要的函式
    ```
    **注意 Sail 的類型對應：** Sail 的 `bits(N)` 通常對應 C 的 `uint64_t` 或 `uint32_t`。`int64` 對應 C 的 `int64_t`。`bool` 對應 C 的 `bool` 或 `uint8_t`。請確保傳遞的型別匹配。

2.  **在 Sail 執行邏輯中呼叫這些 API：**
    在 Sail 模型的指令解碼、執行、內存訪問、分支判斷、寫回等階段，插入對應的 `Perf` API 呼叫。

    * **範例 (概念性，需根據實際 Sail 邏輯調整)：**
        ```sail
        (* 在 riscv_step.sail 函式或處理指令執行的邏輯中 *)
        function step_instruction(): unit = {
            // ... 執行取指、解碼 ...
            let decoded_instr = decode(current_pc);

            // 在指令進入流水線前或執行初期呼叫
            createInstrForStageInfo();

            // ... 執行指令的具體邏輯 ...
            // 範例：如果指令涉及讀取寄存器
            let rs1_val = read_GPR(decoded_instr.rs1);
            let rs2_val = read_GPR(decoded_instr.rs2);
            read_rs_val(sext(rs1_val, 64), sext(rs2_val, 64)); (* 注意 Sail 的類型轉換 *)

            // 範例：如果指令是載入或儲存
            if is_load_store(decoded_instr) then {
                let mem_addr = compute_memory_address(decoded_instr);
                read_addr(sext(mem_addr, 64));
            };

            // 範例：如果指令是 R-type 並寫回寄存器
            if is_R_type(decoded_instr) then {
                // ... 計算結果並寫回 rd ...
                read_Rtype_regs(decoded_instr.rs2, decoded_instr.rs1, decoded_instr.rd);
            };

            // 範例：如果指令是分支，判斷是否跳轉
            if is_branch(decoded_instr) then {
                let branch_taken = predict_branch(decoded_instr);
                read_taken(branch_taken);
            };

            // 在指令完成其微架構執行後呼叫
            sendInstToPipeline();
            // ...
        }
        ```

這使得 `Perf` 成為 `sail-riscv` 功能模擬器的一個效能擴充層，能夠在不改變功能正確性的前提下，提取詳細的微架構行為數據。

---

## 🤝 貢獻

本專案處於積極開發階段，非常歡迎任何形式的貢獻！無論是 Bug 回報、功能建議、程式碼提交，或是關於微架構設計、測試策略和如何**更優雅地將效能模型與 Sail 功能模型結合**的討論，都將對專案的發展至關重要。

---

## 📜 許可證

這個專案遵循 [請在這裡填寫您的許可證，例如 MIT License 或 Apache 2.0 License]。

---