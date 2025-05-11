#pragma once
#include "Player.hpp"
namespace coup{
    class Merchant: public Player{
        public:
            // constructor
            Merchant(const string &name);
            
            // to avoid the class being abstract
            void virtualFunction() {
                // nothing to do here
            }
    };
}