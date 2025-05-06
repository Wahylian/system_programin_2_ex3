#pragma once
#include "Player.hpp"
namespace coup{
    class Governor: public Player{
        public:
            // constructor
            Governor(Game &game, const string &name);

            // the governor can take 3 coins from the bank instead of 2
            void tax();

            // the governor can undo another player's tax action
            void undo(Player &other);
    };
}