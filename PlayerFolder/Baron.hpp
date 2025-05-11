#pragma once
#include "Player.hpp"
namespace coup{
    class Baron: public Player{
        public:
            // constructor
            Baron(const string &name);

            // the player can "invest" 3 coins and receives 6 in return.
            void invest();

            // to avoid the class being abstract
            void virtualFunction() {
                // nothing to do here
            }
    };
}