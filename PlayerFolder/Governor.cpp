#include "Governor.hpp"
namespace coup{
    Governor::Governor(const string &name): Player(name, "Governor") {
        // nothing to do here
    }

    Governor::~Governor() {
        // nothing to do here
        std::cout<< "Governor " << this->name << " destroyed" << std::endl;
    }

    void Governor::tax(){
        int taxAmount = 3; // the amount of coins the governor can tax

        // check if the player has enough actions left
        if(this->remainingActions == 0)
            throw out_of_actions_exception(this->name);

        // check if the tax action is blocked
        if(this->isTaxBlocked())
            throw blocked_action_exception("tax");

        this->addCoins(taxAmount); // increase the coin count by 3

        this->remainingActions--; // decrease the actions left by 1
    }

    void Governor::undoTax(Player &other){
        // checks if other is this player
        if(this == &other)
            throw illegal_action_on_self_exception("Undo Tax");

        // if other is a governor, removes 3 coins from them
        if(other.getRole() == "Governor"){
            other.removeCoins(3);
        } else {
            // if other is not a governor, removes 2 coins from them
            other.removeCoins(2);
        }
    }
}