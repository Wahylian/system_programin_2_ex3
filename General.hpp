#pragma once
#include "Player.hpp"
namespace coup{
    class General: public Player{
        public:
            // constructor
            General(Game &game, const string &name);

            // the general can pay 5 coins to block a coup action against a player (including themselves).
            void blockCoup(Player &other);
    };
}