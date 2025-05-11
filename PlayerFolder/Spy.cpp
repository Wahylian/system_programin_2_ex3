#include "Spy.hpp"
namespace coup{
    Spy::Spy(const string &name): Player(name, "Spy") {
        // nothing to do here
    }

    void Spy::spyOn(Player &other){
        // if the player is this player, throw an excpetion
        if(this == &other)
            throw illegal_action_on_self_exception("Spy On");
        
        // blocks the other player from using the arrest action
        other.blockArrest();
    }
}