#include "Game.hpp"

namespace coup{
    Game::Game(const vector<string> &players) :
        _players{}, _currentPlayer{nullptr} {
        // check if the number of players is valid
        if(players.size() < MIN_PLAYERS || players.size() > MAX_PLAYERS)
            throw invalid_number_of_players_exception(players.size());
        
        // sets the players for the game
        for(const string &playerName : players){
            // create a new player pointer for the current player
            Player *newPlayer = nullptr;

            // roll a random role for this player
            int randomRole = rand() % 6;
            switch(randomRole){
                case 0:
                    newPlayer = new Spy(playerName);
                    break;
                case 1:
                    newPlayer = new Merchant(playerName);
                    break;
                case 2:
                    newPlayer = new Judge(playerName);
                    break;
                case 3:
                    newPlayer = new Governor(playerName);
                    break;
                case 4:
                    newPlayer = new General(playerName);
                    break;
                case 5:
                    newPlayer = new Baron(playerName);
                    break;
            }

            // adds the player to the vector of players
            (this->_players).push_back(newPlayer);
        }

        // sets the current player to the first player
        (this->_currentPlayer) = _players[0];
        
    }

    Game::~Game(){
        // deletes all the players remaining in the game
        for(int i = 0; i< (this->_players).size(); i++){
            // delete the player
            delete (this->_players)[i];

            // set the pointer to null
            (this->_players)[i] = nullptr;
        }
    }

    vector<string> Game::players() const {
        // creates a vector of strings to hold the names of the players
        vector<string> playersNames;
        // goes through all the players in the game
        for(int i = 0; i < (this->_players).size(); i++){
            // adds the name of the player to the vector
            playersNames.push_back((this->_players[i])->getName());
        }
        // returns the vector of player names
        return playersNames;
    }

    string Game::turn() const {
        // returns the name of the current player
        return (this->_currentPlayer)->getName();
    }

    bool Game::playAction(const string &action, Player *target = nullptr){
        // get the current players turn
        Player *currentPlayer = (this->_players)[currentPlayerIndex()];
        // check if the action is valid for the current player
        if(!currentPlayer->isValidAction(action))
            throw invalid_action_exception(action, currentPlayer->getName());

        // check if the player wants to end their turn
        if(action == "endTurn"){
            // end the turn
            currentPlayer->endTurn();
            // advance the game to the next player
            this->nextTurn();
            return true;
        }
        
        // perform the appropriate action for the player
        if(action == "gather")
            currentPlayer->gather();
        if(action == "tax")
            currentPlayer->tax();
        if(action == "bribe"){
            currentPlayer->bribe();
        }
        if(action == "arrest"){
            // check if the target player is valid
            if(target == nullptr)
                throw player_not_in_game_exception(target->getName());

            // try to arrest the target player
            currentPlayer->arrest(*target);
        } 
        if(action == "sanction"){
            // check if the target player is valid
            if(target == nullptr)
                throw player_not_in_game_exception(target->getName());

            // try to sanction the target player
            currentPlayer->sanction(*target);
        }
        if(action == "coup"){
            // check if the target player is valid
            if(target == nullptr)
                throw player_not_in_game_exception(target->getName());

            // try to coup the target player
            currentPlayer->coup(*target);
        }
        
        // check if the player wants to do a specialized action
        // no need to check that the player is of the required roles for these actions,
        // as this is done earlier in the function
        if(action == "invest"){ 
            Baron *baron = (Baron *)currentPlayer;
            baron->invest();
        }
        if(action == "blockCoup"){
            General *general = (General *)currentPlayer;
            general->blockCoup();
        }
        if(action == "spyOn"){
            Spy *spy = (Spy *)currentPlayer;
            spy->spyOn(*target);
        }
        if(action == "blockArrest"){
            Spy *spy = (Spy *)currentPlayer;
            spy->blockOtherArrest(*target);
        }

        // if the action was not to end the turn, return false
        return false;
    }

    void Game::undoAction(const Player& player, const string &undoAction){
        // check if the action is valid for the player
        if(!player.isValidUndoAction(undoAction))
            throw invalid_action_exception(undoAction, player.getName());

        // no need to check that the player is of the required roles for these actions,
        // as this is done earlier in the function
        if(undoAction == "undoBribe"){
            Judge *judge = (Judge *)&player;
            judge->undoBribe(*this->_currentPlayer);
        }
        if(undoAction == "undoTax"){
            Governor *governor = (Governor *)&player;
            governor->undoTax(*this->_currentPlayer);
        }
    }

    void Game::nextTurn(){
        // advance the game to the next player
        int nextPlayerIndex = this->nextPlayerIndex();

        // gets the pointer to the next player
        Player *nextPlayer = (this->_players)[nextPlayerIndex];

        // sets the current player to the next player
        this->_currentPlayer = nextPlayer;

        // returns
        return;
    }

    vector<Player *> Game::allOfRole(const string &role) const{
        // create a vector to hold the players of the specified role
        vector<Player *> playersOfRole;
        // go through all the players in the game
        for(int i = 0; i < (this->_players).size(); i++){
            // check if the player is the current player
            if((this->_players)[i] == this->_currentPlayer)
                continue; // skips the current player automatically

            // check if the player is of the specified role
            if((this->_players)[i]->getRole() == role){
                // add the player to the vector
                playersOfRole.push_back((this->_players)[i]);
            }
        }

        // returns the vector of players of the specified role
        return playersOfRole;
    }

    int Game::playersCount() const{
        // returns the number of players in the game
        return (this->_players).size();
    }

    string Game::winner() const{
        // check if the game is over
        if(!this->isGameOver())
            throw game_not_over_exception();

        // return the name of the winner
        return (this->_currentPlayer)->getName();
    }

    Player* Game::getPlayerByIndex(int index) const{
        // check if the index is out of bounds
        if(index < 0 || index >= (this->_players).size())
            throw std::out_of_range("index out of bounds");

        // return the player at the specified index
        return (this->_players)[index];
    }

    int Game::currentPlayerIndex() const{
        // finds the index of the current player
        for(int i = 0; i < (this->_players).size(); i++){
            // check if the player is the current player
            if((this->_players)[i] == this->_currentPlayer)
                return i; // returns the index of the current player
        }

        // if the current player is not found, throw an exception
        throw current_player_not_found_exception();
    }

    int Game::nextPlayerIndex() const{
        // gets the index of the current player
        int currentPlayerIndex = this->currentPlayerIndex();

        // returns the index of the next player
        return (currentPlayerIndex + 1) % (this->_players).size();
    }

    void Game::removePlayer(const Player &player){
        if(this->_currentPlayer == &player)
            throw deleting_current_player_exception(player.getName());

        // check if the player is in the game
        if(!this->isPlayerInGame(player))
            throw player_not_in_game_exception(player.getName());

        // remove the player from the game
        for(int i = 0; i < (this->_players).size(); i++){
            // check if the player is the one to be removed
            if((this->_players)[i] == &player){
                // gets a pointer to the player to be removed
                Player *temp = (this->_players)[i];
                // remove the player from the vector
                (this->_players).erase(this->_players.begin() + i);

                // delete the player
                delete temp;
                // ends the loop after deletion
                break;
            }
        }
    }

    bool Game::isPlayerInGame(const Player &player) const{
        // check if the player is in the game
        for(int i = 0; i < (this->_players).size(); i++){
            // check if the player is the one to be removed
            if((this->_players)[i] == &player)
                return true; // returns true if the player is in the game
        }

        // returns false if the player is not in the game
        return false;
    }

    bool Game::isGameOver() const{
        // check if the game is over
        if(this->_players.size() == 1)
            return true; // returns true one player remains
        return false; // the game is not over if there is more than one player
    }
}