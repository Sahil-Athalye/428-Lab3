// Game.cpp
// authors: Sahil Athalye
//          Yuheng Ding
//          Reedham Kalariya
// This files defines class methods for Game

#include "Game.h"


Game::Game(int argc, const char * argv[]) {
    for (int i = 2; i < argc; i++) {
        playerNames.push_back(std::string(argv[i]));
    }
}