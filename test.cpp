#include <immintrin.h> // for AVX, AVX2, AVX-512
#include "screen.h"

using namespace std;

void add_vectors(const float* a, const float* b, float* result) {
    __m256 va = _mm256_loadu_ps(a);      // Load 8 floats from memory into a 256-bit register
    __m256 vb = _mm256_loadu_ps(b);      // Load 8 floats from memory into another 256-bit register
    __m256 vr = _mm256_add_ps(va, vb);   // Perform SIMD addition on the 8 floats
    _mm256_storeu_ps(result, vr);        // Store the result back to memory
}

int main() {
    float arr_a[8] = {1,2,3,4.5,5,6.2,7.4,8};
    float arr_b[8] = {3,6,3,4.5,5,6.2,7.4,8};
    float result[8];

    add_vectors(arr_a,arr_b,result);
    for(int i = 0; i<8; i++) {
        cout << result[i] << ", ";
    }
    cout << endl;

    return 0;
}