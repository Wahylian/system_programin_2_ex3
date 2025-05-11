#pragma once
#include "Player.hpp"
namespace coup{
    class Judge: public Player{
        public:
            // constructor
            Judge(const string &name);

            // the judge can undo the bribe action of another player
            void undoBribe(Player &other);
            
            // to avoid the class being abstract
            void virtualFunction() {
                // nothing to do here
            }
    };
}