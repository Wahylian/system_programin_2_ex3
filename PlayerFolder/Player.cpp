#include "Player.hpp"
namespace coup{
    Player::Player(const string &name, string role): name(name), role(role), coin(0) , blockedFrom{{}} , actionsLeft(0) {
        if (role != "Governor" && role != "Spy" && role != "Baron" && role != "General" && role != "Judge" && role != "Merchant") {
            throw std::invalid_argument("Invalid role");
        }

        // adds an appropriate number of falses to the blockedFrom vector
        for(int i=0; i < BLOCKABLE_ACTIONS; i++){
            this->blockedFrom.push_back(false);
        }
    }

    void Player::gather() {
        // check if the player has enough actions left
        if(this->actionsLeft == 0)
            throw out_of_actions_exception(this->name);

        // check if the gather action is blocked
        if(this->isGatherBlocked())
            throw blocked_action_exception("gather");

        // adds 1 coin to the player
        this->addCoins(1); // increase the coin count by 1

        this->actionsLeft--; // decrease the actions left by 1
    }

    void Player::tax(){
        // check if the player has enough actions left
        if(this->actionsLeft == 0)
            throw out_of_actions_exception(this->name);

        // check if the tax action is blocked
        if(this->isTaxBlocked())
            throw blocked_action_exception("tax");

        this->addCoins(2); // increase the coin count by 2

        this->actionsLeft--; // decrease the actions left by 1
    }

    void Player::bribe() {
        int priceOfBribe = 4; // the price of the bribe action

        // checks if the player has enough coins
        if(this->coin < priceOfBribe)
            throw not_enough_coins_exception("bribe", priceOfBribe);
        
        // removes 4 coins from the player
        this->removeCoins(priceOfBribe);

        // perform the bribe action
        this->actionsLeft += 2; // increase the actions left by 2
    }

    void Player::arrest(Player &other){
        // check if the player has enough actions left
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->name);
        
        // check if the arrest action is blocked
        if(this->isArrestBlocked())
            throw blocked_action_exception("arrest");
        
        // check if the player is attempting to arrest themselves
        if(this == &other)
            throw illegal_action_on_self_exception("arrest");

        // check if the last player arrested is the same as the current one
        if(this->isLastArrested(other.getName()))
            throw double_jeopardy_exception(other.getName());
        
        // sets the player as the last arrested player
        this->lastArrested = other.getName();

        // check if the player is a merchent
        if(other.role == "Merchant"){
            // removes 2 coins from the player
            other.removeCoins(2);
        }
        // check if the player is a general
        else if(other.role == "General"){
            // does nothing as the general gets back the coin that is taken from them 
        }
        else{
            // check if the player has enough coins to give this player
            if(other.coins() > 0)
                // add 1 coin to this player
                this->addCoins(1);

            // attempts remove 1 coin from the player
            other.removeCoins(1);
        }
    }

    void Player::sanction(Player &other) {
        int priceOfSanction = 3; // the price of the sanction action

        // check if the player has enough actions left
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->name);

        // check if other is a judge
        if(other.role == "Judge")
            // increase the payment by 1
            priceOfSanction++;

        // checks if the player has enough coins
        if(this->coin < priceOfSanction)
            throw not_enough_coins_exception("sanction", priceOfSanction);
        
        // check if the player is attempting to sanction themselves
        if(this == &other)
            throw illegal_action_on_self_exception("sanction");

        // check if the player is attempting to sanction a baron
        if(other.role == "Baron")
            // if so removes just 1 coin from other
            other.removeCoins(1);
        else
            // removes the right amount of coins from other
            this->removeCoins(2);

        // perform the sanction action
        other.blockTax();
        other.blockGather();

        this->actionsLeft--; // decrease the actions left by 1
    }

    void Player::coup(Player &other) {
        int priceOfCoup = 7; // the price of the coup action

        // check if the player has enough actions left
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->name);

        // checks if the player has enough coins
        if(this->coin < priceOfCoup)
            throw not_enough_coins_exception("coup", priceOfCoup);
        
        // check if the player is attempting to coup themselves
        if(this == &other)
            throw illegal_action_on_self_exception("coup");

        // removes 7 coins from this player
        this->removeCoins(priceOfCoup);
        
        // attempts the coup action

        // check if the coup action is blocked on the other player
        if(other.isCoupBlocked())
            throw blocked_action_exception("coup");
    }

    void Player::endTurn() {
        // remove all actions left
        this->actionsLeft = 0;

        // unblock all actions
        this->unblockCoup();
        this->unblockTax();
        this->unblockGather();
        this->unblockArrest();
    }

    string Player::getName() const {
        return this->name; // return the name of the player
    }

    int Player::coins() const {
        return this->coin; // return the number of coins the player has
    }

    ostream& operator<<(ostream &out, const Player &player) {
        out << "name: " << player.name << endl; // adds the name of the player
        out << "role: " << player.role << endl; // adds the role of the player
        out << "coins: " << player.coin << endl; // adds the number of coins the player has
        return out; // print the player
    }

    void Player::removeActions(int amount) {
        if(amount < 0){
            throw std::invalid_argument("cannot remove a negative amount of actions");
        }
        if(this->actionsLeft < amount){
            this->actionsLeft = 0; // set the actions left to 0
        } else {
            this->actionsLeft -= amount; // remove actions from the player
        }
    }

    void Player::addCoins(int amount){
        if(amount < 0){
            throw std::invalid_argument("cannot add a negative amount of coins");
        }
        this->coin += amount; // add coins to the player
    }

    void Player::removeCoins(int amount){
        if(amount < 0){
            throw std::invalid_argument("cannot remove a negative amount of coins");
        }
        if(this->coin < amount){
            this->coin = 0; // set the coin count to 0
        } else {
            this->coin -= amount; // remove coins from the player
        }
    }


    bool Player::isOutOfActions() const {
        return this->actionsLeft == 0; // check if the player is out of actions
    }

    bool Player::isCoupBlocked() const {
        return this->blockedFrom[0]; // check if the coup action is blocked
    }

    bool Player::isTaxBlocked() const {
        return this->blockedFrom[1]; // check if the tax action is blocked
    }

    bool Player::isGatherBlocked() const {
        return this->blockedFrom[2]; // check if the gather action is blocked
    }

    bool Player::isArrestBlocked() const {
        return this->blockedFrom[3]; // check if the arrest action is blocked
    }

    void Player::blockCoup() {
        this->blockedFrom[0] = true; // block the coup action
    }

    void Player::blockTax() {
        this->blockedFrom[1] = true; // block the tax action
    }

    void Player::blockGather() {
        this->blockedFrom[2] = true; // block the gather action
    }

    void Player::blockArrest() {
        this->blockedFrom[3] = true; // block the arrest action
    }
    
    void Player::unblockCoup() {
        this->blockedFrom[0] = false; // unblock the coup action
    }

    void Player::unblockTax() {
        this->blockedFrom[1] = false; // unblock the tax action
    }

    void Player::unblockGather() {
        this->blockedFrom[2] = false; // unblock the gather action
    }

    void Player::unblockArrest() {
        this->blockedFrom[3] = false; // unblock the arrest action
    }

}