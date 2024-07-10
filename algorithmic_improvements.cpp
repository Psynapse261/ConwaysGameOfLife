#include <array>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
//#include "screen.h"

using namespace std;

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;

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
        // ------------  Finding the next gamestate from the previous gamestate ---------- //
            // Not including the borders
            for(int i = 1; i < GAME_WIDTH - 1; i++) {
                for(int k = 1; k < GAME_HEIGHT - 1; k++) {
                    if(isAlive(display, i, k)){
                        swap[i][k] += 128;
                        swap[i+1][k]++;
                        swap[i-1][k]++;
                        swap[i][k-1]++;
                        swap[i][k+1]++;
                        swap[i+1][k+1]++;
                        swap[i-1][k+1]++;
                        swap[i+1][k-1]++;
                        swap[i-1][k-1]++;
                    }
                }
            }
            // The vertical edges without corners
            for(int i = 1; i < GAME_WIDTH - 1; i++) {
                //Upper edge (k = 0)
                if(isAlive(display, i, 0)) {
                    swap[i][0] += 128;
                    swap[i+1][0]++;
                    swap[i-1][0]++;
                    swap[i][1]++;
                    swap[i+1][1]++;
                    swap[i-1][1]++;
                }
                //Lower edge (k = GAME_HEIGHT - 1)
                if(isAlive(display, i, GAME_HEIGHT - 1)) {
                    swap[i][GAME_HEIGHT - 1] += 128;
                    swap[i+1][GAME_HEIGHT - 1]++;
                    swap[i-1][GAME_HEIGHT - 1]++;
                    swap[i][GAME_HEIGHT - 2]++;
                    swap[i+1][GAME_HEIGHT - 2]++;
                    swap[i-1][GAME_HEIGHT - 2]++;
                }
            }
            // The horizontal edges without corners
            for(int k = 1; k < GAME_HEIGHT - 1; k++) {
                //Left edge (i = 0)
                if(isAlive(display, k, 0)) {
                    swap[0][k] += 128;
                    swap[0][k + 1]++;
                    swap[0][k - 1]++;
                    swap[1][k]++;
                    swap[1][k + 1]++;
                    swap[1][k - 1]++;
                }
                //Right edge (i = GAME_WIDTH - 1)
                if(isAlive(display, k, GAME_HEIGHT - 1)) {
                    swap[GAME_WIDTH - 1][k] += 128;
                    swap[GAME_WIDTH - 1][k + 1]++;
                    swap[GAME_WIDTH - 1][k - 1]++;
                    swap[GAME_WIDTH - 2][k]++;
                    swap[GAME_WIDTH - 2][k + 1]++;
                    swap[GAME_WIDTH - 2][k - 1]++;
                }
            }
            // The four corners
            if(isAlive(display,0,0)) {
                swap[0][0] += 128;
                swap[1][0]++;
                swap[0][1]++;
                swap[1][1]++;
            }
            if(isAlive(display,GAME_WIDTH - 1,0)) {
                swap[GAME_WIDTH - 1][0] += 128;
                swap[GAME_WIDTH - 2][0]++;
                swap[GAME_WIDTH - 1][1]++;
                swap[GAME_WIDTH - 2][1]++;
            }
            if(isAlive(display,0,GAME_HEIGHT - 1)) {
                swap[0][GAME_HEIGHT - 1] += 128;
                swap[1][GAME_HEIGHT - 1]++;
                swap[0][GAME_HEIGHT - 2]++;
                swap[1][GAME_HEIGHT - 2]++;
            }
            if(isAlive(display,GAME_WIDTH - 1,GAME_HEIGHT - 1)) {
                swap[GAME_WIDTH - 1][GAME_HEIGHT - 1] += 128;
                swap[GAME_WIDTH - 2][GAME_HEIGHT - 1]++;
                swap[GAME_WIDTH - 1][GAME_HEIGHT - 2]++;
                swap[GAME_WIDTH - 2][GAME_HEIGHT - 2]++;
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
        copy(swap.begin(), swap.end(), display.begin());

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