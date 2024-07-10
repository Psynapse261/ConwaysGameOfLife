#include <iostream>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
//#include "screen.h"

using namespace std;

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;

bool isAlive(array<array<int, GAME_HEIGHT>, GAME_WIDTH> &game, const int x, const int y) {
    //Tracker for number of neighbouring cells alive for a given cell
    int alive = 0;
    
    //Testing the left cell, only if current cell is not leftmost (x = 0)
    if(x > 0 && game[x-1][y] == 1) alive += 1;
    //Testing right cell
    if(x < GAME_WIDTH && game[x+1][y] == 1) alive += 1;
    //Testing lower cell
    if(y < GAME_HEIGHT && game[x][y+1] == 1) alive += 1;
    //Testing upper cell
    if(y > 0 && game[x][y-1] == 1) alive += 1;
    //Testing top left cell
    if(x > 0 && y > 0 && game[x-1][y-1] == 1) alive += 1;
    //Testing top right cell
    if(x < GAME_WIDTH && y > 0 && game[x+1][y-1] == 1) alive += 1;
    //Testing bottom left cell
    if(x > 0 && y < GAME_HEIGHT && game[x-1][y+1] == 1) alive += 1;
    //Testing bottom right cell
    if(x < GAME_WIDTH && y < GAME_HEIGHT && game[x+1][y+1] == 1) alive += 1;

    //Enforcing the rule to see if current cell is alive or not
    //Rule 1: if cell is alive and has less than 2 alive neighbours, it dies
    if(game[x][y] == 1 && alive < 2) return false;
    //Rule 2: if cell is alive and 2 or 3 neighbours are alive, cell lives
    if(game[x][y] == 1 && (alive == 2 || alive == 3)) return true;
    //Rule 3: if a cell has more than 3 neighbours, cell dies
    if(alive > 3) return false;
    //Rule 4: if 3 neighbours are alive and point is dead, it comes to life
    if(game[x][y] == 0 && alive == 3) return true;

    //default return false
    return false;
}


int main() {
    //Setup
    //G screen;
    array<array<int, GAME_HEIGHT>, GAME_WIDTH> display {};
    array<array<int, GAME_HEIGHT>, GAME_WIDTH> swap {};

    //Creating a random array of starter points
    for(auto& row:display) {
        generate(row.begin(), row.end(), []() {return rand() % 10 == 0 ? 1:0;});
    }

    /* Uint32 start_time, frame_time;
    float fps, avg_fps = 0; */
    int iter = 0;

    //The game loop
    while(iter < 1000) {
        //cout << iter << endl;
        /* start_time = SDL_GetTicks(); */
        //Finding the next gamestate from the previous gamestate
        for(int i = 0; i < GAME_WIDTH; i++) {
            for(int k = 0; k < GAME_HEIGHT; k++) {
                swap[i][k] = isAlive(display, i, k) ? 1:0;
            }
        }

        //Drawing the game state
        for(int i = 0; i < GAME_WIDTH; i++) {
            for(int k = 0; k < GAME_HEIGHT; k++) {
                if(swap[i][k] == 1) {
                    swap[i][k]++;
                }
            }
        }

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
        iter++;
        avg_fps = (avg_fps*iter + fps)/(iter+1); */
        iter++;
    }
    //cout << "Done" << endl;
}