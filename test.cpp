#include <immintrin.h>  // AVX intrinsics header
#include <iostream>

__m256i isAlive_AVX(__m256i cells) {
    
    //Setting the return register
    __m256i result = _mm256_set1_epi8(0x00);
    //Whatever is alive, set it to alive in the result
    __m256i condition = _mm256_set1_epi8(0x88);
    result = _mm256_cmpgt_epi8(cells, condition);
    __m256i ones = _mm256_set1_epi8(0xFF);
    result = _mm256_xor_si256(result, ones);

    //Remove the alive state from the registers
    condition = _mm256_set1_epi8(128);
    condition = _mm256_and_si256(condition, result);
    cells = _mm256_sub_epi8(cells, condition);

    //Whatever has less than 2 neighbours dies
    condition = _mm256_set1_epi8(1);
    __m256i mask = _mm256_cmpgt_epi8(cells, condition);
    result = _mm256_and_si256(result, mask);

    //Anything greater than 3 neighbours dies
    condition = _mm256_set1_epi8(3);
    mask = _mm256_cmpgt_epi8(cells, condition);
    mask = _mm256_xor_si256(mask, ones);
    result = _mm256_and_si256(result, mask);

    //Anything with exactly 3 neighbors lives
    mask = _mm256_cmpeq_epi8(cells, condition);
    result = _mm256_or_si256(result, mask);

    /* //D3ebug
    alignas(32) uint8_t print[32];
    _mm256_store_si256(reinterpret_cast<__m256i*>(print), result);
    std::cout << "Cells: ";
    for(int i = 0; i < 18; i++) {
        std::cout << static_cast<int>(print[i]) << " ";
    }
    std::cout << std::endl; */
    return result;
}

int main() {
    alignas(32) uint8_t data[32], result[32];  // Example data and result arrays

    //Setting example data array
    std::cout << "Initial Cell State: ";
    for(int i = 0; i < 18; i++) {
        data[i] = (i/9)*128 + i%9;
        std::cout << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::endl;


    // Load data into AVX registers
    __m256i avxReg1 = _mm256_load_si256(reinterpret_cast<__m256i*>(data));

    // Perform comparison
    __m256i result_avx = isAlive_AVX(avxReg1);
    
    _mm256_store_si256(reinterpret_cast<__m256i*>(result), result_avx);
    std::cout << "Final Result: ";
    for(int i = 0; i < 18; i++) {
        std::cout << static_cast<int>(result[i]) << " ";
    }
    std::cout << std::endl;


    return 0;
}