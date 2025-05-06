#pragma once
#include "Player.hpp"
namespace coup{
    class Merchant: public Player{
        public:
            // constructor
            Merchant(Game &game, const string &name);
    };
}