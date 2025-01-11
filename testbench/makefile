# 定義目標目錄與腳本
MIBENCH_RESULTS = mibench_results
POLYBENCH_RESULTS = polybench_results
RESULTS_SUMMARY_DIR = results_summary
RESULTS_DETAIL_DIR = results_detail

RUN_MIBENCH_SCRIPT = ./run_scripts/run_mibench.sh
RUN_POLYBENCH_SCRIPT = ./run_scripts/run_polybench.sh
GEN_RESULTS_SCRIPT = python3 ./run_scripts/gen_results.py
GEN_DETAIL_SCRIPT = python3 ./run_scripts/gen_detail.py

# 預設目標
all: mibench polybench summary detail

# 執行 mibench 測試
mibench: $(MIBENCH_RESULTS)
$(MIBENCH_RESULTS):
	@echo "Running mibench tests..."
	$(RUN_MIBENCH_SCRIPT)

# 執行 polybench 測試
polybench: $(POLYBENCH_RESULTS)
$(POLYBENCH_RESULTS):
	@echo "Running polybench tests..."
	$(RUN_POLYBENCH_SCRIPT)

# 匯總結果
summary: $(RESULTS_SUMMARY_DIR)/mibench_summary $(RESULTS_SUMMARY_DIR)/polybench_summary
$(RESULTS_SUMMARY_DIR)/mibench_summary: mibench
	@echo "Generating mibench summary..."
	echo "1" | $(GEN_RESULTS_SCRIPT)

$(RESULTS_SUMMARY_DIR)/polybench_summary: polybench
	@echo "Generating polybench summary..."
	echo "2" | $(GEN_RESULTS_SCRIPT)

# 生成詳細結果
detail: $(RESULTS_DETAIL_DIR)/mibench_detail $(RESULTS_DETAIL_DIR)/polybench_detail
$(RESULTS_DETAIL_DIR)/mibench_detail: summary
	@echo "Generating mibench detail..."
	echo "1" | $(GEN_DETAIL_SCRIPT)

$(RESULTS_DETAIL_DIR)/polybench_detail: summary
	@echo "Generating polybench detail..."
	echo "2" | $(GEN_DETAIL_SCRIPT)

# 清理
clean:
	@echo "Cleaning up generated files..."
	rm -rf $(MIBENCH_RESULTS) $(POLYBENCH_RESULTS) \
	$(RESULTS_SUMMARY_DIR)/* $(RESULTS_DETAIL_DIR)/*
