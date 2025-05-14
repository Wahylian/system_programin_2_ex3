#include "Baron.hpp"
namespace coup{
    Baron::Baron(const string &name): Player(name, "Baron") {
        // nothing to do here
    }

    Baron::~Baron() {
        // nothing to do here
        std::cout<< "Baron " << this->name << " destroyed" << std::endl;
    }

    void Baron::invest(){
        int priceOfInvestment = 3; // the price of the invest action

        // checks if the player has enough coins
        if(this->coin < priceOfInvestment)
            throw not_enough_coins_exception("invest", priceOfInvestment);

        // perform the invest action
        this->addCoins(priceOfInvestment); // increase the coin count by 3
    }
}