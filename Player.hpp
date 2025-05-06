#pragma once
using namespace std;
#include "Game.hpp"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;
namespace coup{
    class Player{
        protected:
            Game &game;
            string name;
            string role;
            int coin;

        public:
            // constructor
            Player(Game &game, const string &name);

            // the player receives 1 coin from the bank, this action costs nothing and can be blocked via the "sanction" action
            void gather();

            // the player takes 2 coins from the bank, this action costs nothing but can be blocked via certain roles or actions
            virtual void tax();

            // the player pays 4 coins to preform 2 actions
            void bribe();

            // the player chooses another player and takes a coin from them.
            // it cannot be used on the same player twice in a row
            void arrest(Player &other);

            // the player chooses another player and blocks them from using economical actions (gather and tax) until their next turn.
            // this action costs 3 coins.
            void sanction(Player &other);

            // the player chooses another player and eliminates them from the game completely.
            // this action costs 7 coins and can be blocked under specific conditions.
            void coup(Player &other);

            // gets the name of the player
            string getName() const;

            // gets the number of coins the player has
            int coins() const;

    };
}