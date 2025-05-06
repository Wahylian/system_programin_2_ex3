#pragma once
#include "Player.hpp"
namespace coup{
    class Judge: public Player{
        public:
            // constructor
            Judge(Game &game, const string &name);

            // the judge can undo the bribe action of another player
            void undo(Player &other);
    };
}