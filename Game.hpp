#pragma once
#include "PlayerFolder/Player.hpp"
#include "PlayerFolder/Spy.hpp"
#include "PlayerFolder/Merchant.hpp"
#include "PlayerFolder/Judge.hpp"
#include "PlayerFolder/Governor.hpp"
#include "PlayerFolder/General.hpp"
#include "PlayerFolder/Baron.hpp"
#include "CustomExceptions/CustomGameExceptions.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <random>
using namespace std;
namespace coup{
    class Game{
        private:
            // the minimal number of players in the game
            const int MIN_PLAYERS = 2;
            // the maximal number of players in the game
            const int MAX_PLAYERS = 6;

            // the vector of players in the game
            vector<Player *> _players;
            // a pointer to the current player
            Player *_currentPlayer;
        public:
            // Constructor
            Game(const vector<string> &players);

            // Destructor
            ~Game();

            // getting the players of the game
            vector<string> players() const;

            // getting the name of the current player
            string turn() const;

            // getting the role of the current player
            string role() const;
            // getting the name of the player, their role, the number of coins they have and
            // the number of actions they have left
            // NOTE: the order is as follows: 0: name, 1: role, 2: coins, 3: actions
            vector<string> info() const;

            // lets the current player do an action
            // note: not all actions are available to all players and not all actions require a target player
            // return -1 if the action was to end the turn, return -2 if the action was not to end the turn
            // if the action was to spyOn another player, return the number of coins the other player has
            // (since the number of coins is non-negative, it is distinguishable from -1 and -2)
            int playAction(const string &action, Player *target = nullptr);


            // lets a player undo an action of the current player
            // note: not all actions are undoable and not all players can undo actions
            void undoAction(const Player& player, const string &undoAction);

            // advances the game to the next player
            void nextTurn();

            // returns true if the action provided is a valid action for the current player, else false
            // NOTE: in the event the current player has at least 10 coins, they must coup another player
            bool isValidAction(const string &action) const;

            // prints the actions available to the current player
            void printValidActions() const;

            // returns all players in the game with the provided role, 
            // except for the current player
            vector<Player*> allOfRole(const string &role) const;

            // getting the number of players still in the game
            int playersCount() const;

            // getting the name of the winner
            string winner() const;

            // returns a pointer to the player at the provided index
            // if the index is out of bounds, it will throw an exception
            Player *getPlayerByIndex(int index) const;  

            // returns true if the action provided requires a target player, else false
            // will throw an exception if the action is not valid
            bool isTargetRequired(const string &action) const;

        private:
            // gets the index of the current player
            int currentPlayerIndex() const;

            // gets the index of the next player
            int nextPlayerIndex() const;

            // removes a player from the game during a coup
            void removePlayer(const Player &player);

            // checks if the player is in the game
            bool isPlayerInGame(const Player &player) const;

            // returns true if the action provided is a valid action for the current player
            //bool isValidAction(const string &action) const;

            // return true if the game is over, else false
            bool isGameOver() const;
    };
}