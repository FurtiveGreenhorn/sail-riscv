# import os
# import re

# # 定義 result 資料夾路徑
# result_folder = "./result"
# output_file = "summary_with_miss_rate.txt"

# # 定義正則表達式以提取所需數據
# patterns = {
#     "Instruction Count": r"Instruction Count: (\d+)",
#     "Cycle Count": r"Cycle Count: (\d+)",
#     "Icache miss rate": r"L1 Icache.*?miss rate: ([\d\.]+)%",
#     "Dcache miss rate": r"L1 Dcache.*?miss rate: ([\d\.]+)%",
#     "L2 cache miss rate": r"L2 cache.*?miss rate: ([\d\.]+)%"
# }

# # 初始化結果列表
# summary = []

# # 遍歷 result 資料夾中的所有檔案
# for filename in os.listdir(result_folder):
#     filepath = os.path.join(result_folder, filename)
#     if os.path.isfile(filepath):
#         with open(filepath, 'r') as file:
#             content = file.read()
        
#         # 提取檔案名作為標題
#         title = os.path.splitext(filename)[0]
#         entry = { "title": title }
        
#         # 匹配數據
#         for key, pattern in patterns.items():
#             match = re.search(pattern, content, re.DOTALL)
#             if match:
#                 entry[key] = match.group(1) + "%" if "miss rate" in key else match.group(1)
#             else:
#                 entry[key] = "N/A"  # 如果未找到，標記為 N/A
        
#         summary.append(entry)

# # 將結果寫入到輸出檔案
# with open(output_file, 'w') as outfile:
#     for entry in summary:
#         outfile.write(f"{entry['title']}:\n")
#         for key, value in entry.items():
#             if key != "title":
#                 outfile.write(f"{key}: {value}\n")
#         outfile.write("\n")

# print(f"Summary written to {output_file}")

import os
import re

# 定義 result 資料夾路徑
result_folder = "./results"
output_file = "./results_summary/results.txt"

# 定義正則表達式以提取所需數據
patterns = {
    "Instruction Count": r"Instruction Count: (\d+)",
    "Cycle Count": r"Cycle Count: (\d+)",
    "Icache miss rate": r"L1 Icache.*?miss rate: ([\d\.]+%)",
    "Dcache miss rate": r"L1 Dcache.*?miss rate: ([\d\.]+%)",
    "L2 cache miss rate": r"L2 cache.*?miss rate: ([\d\.]+%)",
    
    # 新增的正則表達式
    "Latency by cache": r"Latency by cache: (\d+)",
    "Latency by Multiplier Unit": r"Latency by Multiplier Unit: (\d+)",
    "Latency by Divider Unit": r"Latency by Divider Unit: (\d+)",
    "CPI": r"CPI: ([\d\.]+%)",
    "Cache Latency in Total Cycles": r"Cache Latency in Total Cycles: ([\d\.]+%)",
    "Multiplier Unit Latency in Total Cycles": r"Multiplier Unit Latency in Total Cycles: ([\d\.]+%)",
    "Divider Unit Latency in Total Cycles": r"Divider Unit Latency in Total Cycles: ([\d\.]+%)"
}

# 初始化結果列表
summary = []

# 遍歷 result 資料夾中的所有檔案
for filename in os.listdir(result_folder):
    filepath = os.path.join(result_folder, filename)
    if os.path.isfile(filepath):
        with open(filepath, 'r') as file:
            content = file.read()
        
        # 提取檔案名作為標題
        title = os.path.splitext(filename)[0]
        entry = {"title": title}
        
        # 匹配數據
        for key, pattern in patterns.items():
            match = re.search(pattern, content, re.DOTALL)
            if match:
                entry[key] = match.group(1)
            else:
                entry[key] = "N/A"  # 如果未找到，標記為 N/A
        
        summary.append(entry)

# 將結果寫入到輸出檔案
with open(output_file, 'w') as outfile:
    for entry in summary:
        outfile.write(f"{entry['title']}:\n")
        for key, value in entry.items():
            if key != "title":
                outfile.write(f"{key}: {value}\n")
        outfile.write("\n")

print(f"Summary written to {output_file}")
