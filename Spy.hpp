#pragma once
#include "Player.hpp"
namespace coup{
    class Spy: public Player{
        public:
            // constructor
            Spy(Game &game, const string &name);

            // the spy can see the amount of coins of another player and stop them from using the "arrest" action on their next turn.
            // this action costs nothing and doesn't take a turn
            void spyOn(Player &other);
    };
}