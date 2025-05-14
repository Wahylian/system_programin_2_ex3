#pragma once
#include "Player.hpp"
namespace coup{
    class General: public Player{
        public:
            // constructor
            General(const string &name);

            // destructor
            ~General();

            // the general can pay 5 coins to block a coup action against a player (including themselves).
            void blockCoupAttempt(Player &other);

        protected:    
            // to avoid the class being abstract
            void virtualFunction() {
                // nothing to do here
            }
    };
}