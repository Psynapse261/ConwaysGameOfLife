#include <array>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
//#include "screen.h"

using namespace std;

const int GAME_WIDTH = 1024;
const int GAME_HEIGHT = 1024;

bool isAlive(array<array<uint8_t, GAME_HEIGHT>, GAME_WIDTH> &game, const int x, const int y) {
    
    //Giving a quick exit for dead cells without enough neighbours
    if(game[x][y] < 3) return false;
    //Rule 4: if 3 neighbours are alive and point is dead, it comes to life (ie. cell value is 3)
    if(game[x][y] == 3) return true;

    //removing cell state from the value
    game[x][y] & 0b01111111;
    
    //Enforcing the rule to see if current cell is alive or not
    //Rule 1: if cell is alive and has less than 2 alive neighbours, it dies (ie. cell value should be 128/129)
    if(game[x][y] < 2) return false;
    //Rule 2: if cell is alive and 2 or 3 neighbours are alive, cell lives (ie. cell value should be 130/131)
    if(game[x][y] < 4) return true;
    //Rule 3: if a cell has more than 3 neighbours, cell dies (ie. cell value is 4-127 or 132+)
    return false;

}


int main() {
    //Setup
    //G screen;
    array<array<uint8_t, GAME_HEIGHT>, GAME_WIDTH> display {};

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
        array<array<uint8_t, GAME_HEIGHT>, GAME_WIDTH> swap {};

        /* start_time = SDL_GetTicks(); */
        //Finding the next gamestate from the previous gamestate
        for(int i = 0; i < GAME_WIDTH; i++) {
            for(int k = 0; k < GAME_HEIGHT; k++) {
                if(isAlive(display, i, k)){
                    swap[i][k] += 128;
                    if(i+1 < GAME_WIDTH) swap[i+1][k]++;
                    if(i > 0) swap[i-1][k]++;
                    if(k > 0) swap[i][k-1]++;
                    if(k+1 < GAME_HEIGHT) swap[i][k+1]++;
                    if(k+1 < GAME_HEIGHT && i < GAME_WIDTH)swap[i+1][k+1]++;
                    if(i > 0 && k+1 < GAME_HEIGHT) swap[i-1][k+1]++;
                    if(i+1<GAME_HEIGHT && k>0) swap[i+1][k-1]++;
                    if(i>0 && k>0) swap[i-1][k-1]++;
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