#pragma once
#include "Player.hpp"
namespace coup{
    class Baron: public Player{
        public:
            // constructor
            Baron(Game &game, const string &name);

            // the player can "invest" 3 coins and receives 6 in return.
            void invest();
    };
}