#include "General.hpp"
namespace coup{
    General::General(const string &name): Player(name, "General") {
        // nothing to do here
    }

    General::~General() {
        // nothing to do here
    }

    void General::undoCoup(Player &other){
        // undo another player from being coup'd

        // in reality, this function does nothing, as the actual coup action is performed by the game itself
    }
}