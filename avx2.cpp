#include <array>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <immintrin.h> // for AVX, AVX2, AVX-512
#include <iostream>
//#include "screen.h"

using namespace std;

const int GAME_WIDTH = 32;
const int GAME_HEIGHT = 32;

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
    //Setup
    //G screen;
    alignas(32) array<array<uint8_t, GAME_HEIGHT>, GAME_WIDTH> display {};

    //Creating a random array of starter points
    for(int i = 0; i < GAME_WIDTH; i++) {
        for(int k = 0; k < GAME_HEIGHT; k++) {
            if(rand() % 10 == 0) {
                display[i][k] += 128;
                if(i+1 < GAME_WIDTH) display[i+1][k]++;
                if(i > 0) display[i-1][k]++;
                if(k > 0) display[i][k-1]++;
                if(k+1 < GAME_HEIGHT) display[i][k+1]++;
                if(k+1 < GAME_HEIGHT && i < GAME_WIDTH)display[i+1][k+1]++;
                if(i > 0 && k+1 < GAME_HEIGHT) display[i-1][k+1]++;
                if(i+1<GAME_HEIGHT && k>0) display[i+1][k-1]++;
                if(i>0 && k>0) display[i-1][k-1]++;
            }
        };
    }

    /* Uint32 start_time, frame_time;
    float fps, avg_fps = 0; */
    int iter = 0;

    //The game loop
    while(iter < 1000) {
        alignas(32) array<array<uint8_t, GAME_HEIGHT>, GAME_WIDTH> swap {};

        /* start_time = SDL_GetTicks(); */
        //Finding the next gamestate from the previous gamestate
        for(int i = 0; i < GAME_WIDTH; i = i + 32) {
            for(int k = 0; k < GAME_HEIGHT; k++) {
                // Doing AVX magic
                uint8_t result[32];
                __m256i my_cells = _mm256_load_si256(reinterpret_cast<__m256i*>(&display[i][k]));
                __m256i is_alive = isAlive_AVX(my_cells);
                _mm256_store_si256(reinterpret_cast<__m256i*>(result), is_alive);
                
                //Setting the new array values
                for(int cell = 0; cell < 32; cell++) {
                    cout << i + cell << endl;
                    if(result[cell] > 1) {
                    int i_new = i + cell;
                    swap[i + cell][k] += 128;
                    if(i_new+1 < GAME_WIDTH) swap[i_new+1][k]++;
                    if(i_new > 0) swap[i_new-1][k]++;
                    if(k > 0) swap[i_new][k-1]++;
                    if(k+1 < GAME_HEIGHT) swap[i_new][k+1]++;
                    if(k+1 < GAME_HEIGHT && i_new < GAME_WIDTH)swap[i_new+1][k+1]++;
                    if(i_new > 0 && k+1 < GAME_HEIGHT) swap[i_new-1][k+1]++;
                    if(i_new+1<GAME_HEIGHT && k>0) swap[i_new+1][k-1]++;
                    if(i_new>0 && k>0) swap[i_new-1][k-1]++;
                }
                }
            }
        }

        /* //Drawing the game state
        for(int i = 0; i < GAME_WIDTH; i++) {
            for(int k = 0; k < GAME_HEIGHT; k++) {
                if(swap[i][k] >= 128) {
                    screen.drawpixel(i,k);
                }
            }
        } */

        //Swapping buffers
        //copy(swap.begin(), swap.end(), display.begin());

        /* //Display to the screen
        screen.update();
        //SDL_Delay(20);
        screen.input();
        screen.clearpixels(); */
        
        /* //Calculating and printing fps
        frame_time = SDL_GetTicks()-start_time;
        fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;
        avg_fps = (avg_fps*iter + fps)/(iter+1); */
        //std::cout << iter << endl;
        iter++;
    }
    /* std::cout << avg_fps << endl; */
}