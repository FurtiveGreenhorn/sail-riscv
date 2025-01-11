import os
import re

# 提示使用者選擇檔案
def get_filename():
    print("請選擇檔案：")
    print("1. mibench")
    print("2. polybench")
    
    choice = input("請輸入選項 (1 或 2): ")
    
    if choice == '1':
        return 'results_summary/mibench_summary', 'results_detail/mibench_detail'
    elif choice == '2':
        return 'results_summary/polybench_summary', 'results_detail/polybench_detail'
    else:
        print("無效選項，請重新選擇。")
        return get_filename()

# 確保目錄存在
def ensure_directory_exists(filepath):
    directory = os.path.dirname(filepath)
    if not os.path.exists(directory):
        os.makedirs(directory)

# 解析資料
def parse_data(filename):
    results = {
        'Instruction Count': {},
        'Cycle Count': {},
        'Icache miss rate': {},
        'Dcache miss rate': {},
        'L2 cache miss rate': {},
        'Latency by cache': {},
        'Latency by Multiplier Unit': {},
        'Latency by Divider Unit': {},
        'CPI': {},
        'Cache Latency in Total Cycles': {},
        'Multiplier Unit Latency in Total Cycles': {},
        'Divider Unit Latency in Total Cycles': {}
    }
    
    with open(filename, 'r') as file:
        current_program = None
        for line in file:
            # 匹配程序名稱
            match_program = re.match(r'^([a-zA-Z0-9_]+):$', line.strip())
            if match_program:
                current_program = match_program.group(1)
                continue
            
            # 匹配指標數據
            for metric in results:
                match_metric = re.match(rf'^{metric}:\s*([\d.]+)%?', line.strip())
                if match_metric and current_program:
                    results[metric][current_program] = match_metric.group(1)
                    break
    
    return results

# 將結果寫入檔案
def write_results(results, output_file):
    ensure_directory_exists(output_file)
    with open(output_file, 'w') as file:
        for metric, data in results.items():
            file.write(f'{metric}\n')
            for program, value in sorted(data.items()):
                file.write(f'{program}: {value}\n')
            file.write('\n')

# 主程式
def main():
    input_file, output_file = get_filename()
    results = parse_data(input_file)
    write_results(results, output_file)
    print(f"結果已寫入到 {output_file}")

if __name__ == '__main__':
    main()
