#include "Judge.hpp"
namespace coup{
    Judge::Judge(const string &name): Player(name, "Judge") {
        // nothing to do here
    }

    Judge::~Judge() {
        // nothing to do here
        std::cout<< "Judge " << this->name << " destroyed" << std::endl;
    }

    void Judge::undoBribe(Player &other){
        // checks if other is this player
        if(this == &other)
            throw illegal_action_on_self_exception("Undo Bribe");

        // removes the additional action from the other player
        other.remainingActions -= 1;
    }

}