#include "General.hpp"
namespace coup{
    General::General(const string &name): Player(name, "General") {
        // nothing to do here
    }

    General::~General() {
        // nothing to do here
        std::cout<< "General " << this->name << " destroyed" << std::endl;
    }

    void General::blockCoupAttempt(Player &other){
        // blocks another player from being coup'd
        other.blockCoup();
    }
}