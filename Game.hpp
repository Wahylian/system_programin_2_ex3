#pragma once
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;
namespace coup{
    class Game{
        public:
            // Constructor
            Game();

            // getting the players of the game
            vector<string> players() const;

            // getting the name of the current player
            string turn() const;

            // getting the name of the winner
            string winner() const;
    };
}