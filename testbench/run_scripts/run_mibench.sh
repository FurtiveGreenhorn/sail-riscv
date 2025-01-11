
#!/bin/bash

SIMULATOR="../c_emulator/riscv_sim_RV64 -V -p"
MIBENCH_DIR="mibench"
INPUTS_DIR="mibench_inputs"
RESULT_DIR="mibench_results"

# 判斷資料夾是否存在，若不存在則創建
if [ ! -d "$RESULT_DIR" ]; then
  echo "資料夾 $RESULT_DIR 不存在，正在創建..."
  mkdir "$RESULT_DIR"
fi  # 關閉 if 區塊

# echo "running basicmath_small..."
# $SIMULATOR "${MIBENCH_DIR}/automotive/basicmath/basicmath_small"  > "${RESULT_DIR}/basicmath_small.log" 2>&1

echo "running bitcount..."
$SIMULATOR "${MIBENCH_DIR}/automotive/bitcount/bitcnts" 75000 > "${RESULT_DIR}/bitcnts.log" 2>&1

echo "running qsort_small..."
$SIMULATOR "${MIBENCH_DIR}/automotive/qsort/qsort_small" "${INPUTS_DIR}/input_small.dat" > "${RESULT_DIR}/qsort_small.log" 2>&1

echo "running susan_smoothing..."
$SIMULATOR "${MIBENCH_DIR}/automotive/susan/susan" "${INPUTS_DIR}/input_small.pgm" "${INPUTS_DIR}/output_small.smoothing.pgm" -s > "${RESULT_DIR}/susan_smoothing.log" 2>&1

echo "running susan_edges..."
$SIMULATOR "${MIBENCH_DIR}/automotive/susan/susan" "${INPUTS_DIR}/input_small.pgm" "${INPUTS_DIR}/output_small.edges.pgm" -e > "${RESULT_DIR}/susan_edges.log" 2>&1

echo "running susan_corners..."
$SIMULATOR "${MIBENCH_DIR}/automotive/susan/susan" "${INPUTS_DIR}/input_small.pgm" "${INPUTS_DIR}/output_small.corners.pgm" -c > "${RESULT_DIR}/susan_corners.log" 2>&1

echo "running dijkstra_small..."
$SIMULATOR "${MIBENCH_DIR}/network/dijkstra/dijkstra_small" "${INPUTS_DIR}/input.dat" > "${RESULT_DIR}/dijkstra_small.log" 2>&1

echo "running search_small..."
$SIMULATOR "${MIBENCH_DIR}/office/stringsearch/search_small" > "${RESULT_DIR}/search_small.log" 2>&1

echo "running search_large..."
$SIMULATOR "${MIBENCH_DIR}/office/stringsearch/search_large" > "${RESULT_DIR}/search_large.log" 2>&1

echo "running rijndael_enc..."
$SIMULATOR "${MIBENCH_DIR}/security/rijndael/rijndael" "${INPUTS_DIR}/input_small.asc" "${INPUTS_DIR}/output_small.enc" e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321 > "${RESULT_DIR}/rijndael_enc.log" 2>&1

echo "running rijndael_dec..."
$SIMULATOR "${MIBENCH_DIR}/security/rijndael/rijndael" "${INPUTS_DIR}/output_small.enc" "${INPUTS_DIR}/output_small.dec" d 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321 > "${RESULT_DIR}/rijndael_dec.log" 2>&1


echo "running sha..."
$SIMULATOR "${MIBENCH_DIR}/security/sha/sha" "${INPUTS_DIR}/input_small.asc" > "${RESULT_DIR}/sha.log" 2>&1

echo "running CRC32..."
$SIMULATOR "${MIBENCH_DIR}/telecomm/CRC32/crc" "${INPUTS_DIR}/small.pcm" > "${RESULT_DIR}/CRC32.log" 2>&1

echo "running rawcaudio..."
$SIMULATOR "${MIBENCH_DIR}/telecomm/adpcm/rawcaudio" < "${INPUTS_DIR}/small.pcm" > "${RESULT_DIR}/rawcaudio.log" 2>&1

echo "running rawdaudio..."
$SIMULATOR "${MIBENCH_DIR}/telecomm/adpcm/rawdaudio" < "${INPUTS_DIR}/small.adpcm" > "${RESULT_DIR}/rawdaudio.log" 2>&1

echo "running FFT..."
$SIMULATOR "${MIBENCH_DIR}/telecomm/FFT/fft" 4 4096 > "${RESULT_DIR}/FFT.log" 2>&1

echo "running IFFT..."
$SIMULATOR "${MIBENCH_DIR}/telecomm/FFT/fft" 4 8192 -i > "${RESULT_DIR}/IFFT.log" 2>&1














