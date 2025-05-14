#include "Player.hpp"

namespace coup{
    Player::Player(const string &name, const string &role): 
        name{name}, role{role}, coin{0}, 
        remainingActions{0}, lastArrested{nullptr}, blockedActions(BLOCKABLE_ACTIONS, 0){
        // nothing to do in the actual constructor
    }

    Player::Player(const Player &other): 
        name{other.name}, role{other.role}, coin{other.coin}, 
        remainingActions{other.remainingActions}, lastArrested{other.lastArrested}, blockedActions(other.blockedActions){
        // nothing to do in the copy constructor
    }

    Player::~Player() {
        std::cout<< "Player " << this->name << " destroyed" << std::endl;

        // sets lastArrested to null to avoid a pointer to a deleted object
        this->lastArrested = nullptr; 
    }

    string Player::getName() const {
        // return the name of the player
        return this->name; 
    }

    string Player::getRole() const {
        // return the role of the player
        return this->role; 
    }

    int Player::coins() const {
        // return the number of coins the player has
        return this->coin; 
    }

    unsigned int Player::getRemainingActions() const {
        // return the number of actions left for the player to perform in this turn
        return this->remainingActions; 
    }

    const Player& Player::getLastArrested() const {
        // return the last player arrested by this player
        return *this->lastArrested; 
    }

    bool Player::isOutOfActions() const {
        // check if the player has any more actions left
        return this->remainingActions == 0; 
    }

    void Player::gather(){
        // check if the playe has an action remaining
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->name);
        // check if the gather action is blocked for this player
        if(this->isGatherBlocked())
            throw blocked_action_exception("gather");
        
        // adds 1 coin to this player
        this->coin += 1;
        // decrease the actions left for this player by 1
        this->remainingActions--;
    }

    void Player::tax(){
        // check if the player has an action remaining
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->name);
        // check if the tax action is blocked for this player
        if(this->isTaxBlocked())
            throw blocked_action_exception("tax");
        
        // adds 2 coins to this player
        this->coin += 2;
        // decrease the actions left for this player by 1
        this->remainingActions--;
    }

    void Player::bribe(){
        int bribeCost = 4; // the cost of the bribe action is 4 coins
        // check if the player

        // check if the player has enough coins for this action
        if(this->coin < bribeCost)
            throw not_enough_coins_exception("bribe", bribeCost);
        
        // removes 4 coins from this player
        this->coin -= bribeCost;

        // increase the actions left for this player by 1
        this->remainingActions++;
    }

    void Player::arrest(Player &other){
        // check that the player has enough actions left
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->name);

        // check if the arrest action is blocked for this player
        if(this->isArrestBlocked())
            throw blocked_action_exception("arrest");

        // check if this player is attempting to arrest the same player twice in a row
        if(this->lastArrested == &other)
            throw double_jeopardy_exception(other.getName());
        
        // check if this player is attempting to arrest themselves
        if(this == &other)
            throw illegal_action_on_self_exception("arrest");

        // if all the checks were passed
        // arrests the other player
        
        // the arrest action acts differently depending on the role of other
        // if the other player is a merchant, they have to pay 2 to the bank and nothing to this player
        if(other.getRole() == "Merchant"){
            // removes 2 coins from this player
            other.removeCoins(2);
        }
        // if the other player is a general, they dont pay anything
        else if(other.getRole() == "General"){
            // nothing to do here
        }
        // if the other player has any other role, they pay the fine required
        // if they have enough coins that is
        else if (other.coins() > 0){
            // removes 1 coin from the other player and adds it to this player
            other.removeCoins(1);

            // adds 1 coin to this player
            this->coin += 1;   
        }

        // sets the last arrested player to the other player
        this->lastArrested = &other;
        // decrease the actions left for this player by 1
        this->remainingActions--;  
    }

    void Player::sanction(Player &other){
        int sanctionCost = 3; // the price of a sanction is 3 coins

        // check if the player has an action remaining
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->name);

        // if the other player is a judge, the price of the sanction increases by 1
        if(other.getRole() == "Judge")
            sanctionCost++;
        
        // check if the player has enough coins for this action
        if(this->coin < sanctionCost)
            throw not_enough_coins_exception("sanction", sanctionCost);
        
        // check if the other player is this player
        if(this == &other)
            throw illegal_action_on_self_exception("sanction");

        // makes the player pay the sanction
        this->coin -= sanctionCost;

        // if the other player is a baron reimburses them 1 coin
        if(other.getRole() == "Baron"){
            other.addCoins(1);
        }

        // blocks the other player from using economical actions until the end of their next turn
        other.blockGather();
        other.blockTax();

        // decrease the actions left for this player by 1
        this->remainingActions--;
    }

    void Player::coup(const Player &other){
        int coupCost = 7; // the price of a coup is 7 coins

        // check if the player has an action remaining
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->name);

        // check if the player has enough coins for this action
        if(this->coin < 7)
            throw not_enough_coins_exception("coup", coupCost);

        // check if the player is attempting to coup themselves
        if(this == &other)
            throw illegal_action_on_self_exception("coup");

        // check if the other player has a coup block on them
        if(other.isCoupBlocked())
            throw blocked_action_exception("coup");

        // removes the correct amount of coins from this player
        this->coin -= coupCost;
        // remove an action from the player
        this->remainingActions--;
    }

    void Player::endTurn(){
        // removes all actions left for this player
        this->remainingActions = 0;

        // lifts all blocks on this player
        for(int i = 0; i < BLOCKABLE_ACTIONS; i++){
            this->blockedActions[i] = 0;
        }
    }

    void Player::prepareForTurn(){
        // gives the player an action
        this->remainingActions = 1;
    }

    bool Player::isCoupBlocked() const {
        // returns true if the coup action is blocked, else false
        return this->blockedActions[0];
    }

    bool Player::isTaxBlocked() const {
        // returns true if the tax action is blocked, else false
        return this->blockedActions[1];
    }

    bool Player::isGatherBlocked() const {
        // returns true if the gather action is blocked, else false
        return this->blockedActions[2];
    }

    bool Player::isArrestBlocked() const {
        // returns true if the arrest action is blocked, else false
        return this->blockedActions[3];
    }

    Player& Player::operator=(const Player &other){
        // check if this is a self assignment
        if(this == &other)
            return *this; // return a reference to this object
        
        // copies over the values from the other player
        this->name = other.name;
        this->role = other.role;
        this->coin = other.coin;
        this->remainingActions = other.remainingActions;
        this->lastArrested = other.lastArrested;
        // copies over the blocked actions
        for(int i = 0; i < BLOCKABLE_ACTIONS; i++){
            this->blockedActions[i] = other.blockedActions[i];
        }

        // returns a reference to this object
        return *this;
    }

    ostream& operator<<(ostream &out, const Player &player){
        // adds the name of the player
        out << "name: " << player.getName() << endl;
        // adds the role of the player
        out << "role: " << player.getRole() << endl;
        // adds the number of coins the player has 
        out << "coins: " << player.coins() << endl;
        // adds the number of actions left for the player
        out << "actions left: " << player.getRemainingActions() << endl;
        // returns the output stream
        return out;
    }

    void Player::addCoins(int amount){
        // check if the amount is negative
        if(amount < 0)
            throw std::invalid_argument("cannot add a negative amount of coins");
        
        // adds coins to the player
        this->coin += amount;
    }

    void Player::removeCoins(int amount){
        // check if the amount is negative
        if(amount < 0)
            throw std::invalid_argument("cannot remove a negative amount of coins");
        
        // check if the player has enough coins
        if(this->coin < amount)
            this->coin = 0; // set the coin count to 0
        else
            this->coin -= amount; // remove coins from the player
    }

    void clearLastArrested(Player &player){
        // sets the last arrested player to null
        player.lastArrested = nullptr;
    }

    void Player::blockCoup(){
        // sets the coup action as blocked
        this->blockedActions[0] = 1;
    }

    void Player::blockTax(){
        // sets the tax action as blocked
        this->blockedActions[1] = 1;
    }

    void Player::blockGather(){
        // sets the gather action as blocked
        this->blockedActions[2] = 1;
    }
    
    void Player::blockArrest(){
        // sets the arrest action as blocked
        this->blockedActions[3] = 1;
    }
}