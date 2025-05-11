#pragma once
using namespace std;
#include "Game.hpp"

#include "../CustomExceptions.hpp"
#include <iostream>
#include <vector>
using namespace std;
namespace coup{
    class Player{
        protected:
            const int BLOCKABLE_ACTIONS = 4; // there are 4 actions that can be outright blocked: coup, tax, gather and arrest
            // their order in the array is as mentioned above
            string name; // the name of the player
            string role; // the role of the player
            int coin; // the number of coins the player has
            string lastArrested; // the name of the last player arrested by this player
            unsigned int actionsLeft; // the number of actions left for the player to perform in a turn
            vector<bool> blockedFrom; // blocked from using action

        public:
            // constructor
            Player(const string &name, string role);

            // the player receives 1 coin from the bank, this action costs nothing and can be blocked via the "sanction" action
            void gather();

            // the player takes 2 coins from the bank, this action costs nothing but can be blocked via certain roles or actions
            virtual void tax();

            // the player pays 4 coins to preform 2 actions
            void bribe();

            // the player chooses another player and takes a coin from them.
            // it cannot be used on the same player twice in a row
            void arrest(Player &other);

            // the player chooses another player and blocks them from using economical actions (gather and tax) until their next turn.
            // this action costs 3 coins.
            void sanction(Player &other);

            // the player chooses another player and eliminates them from the game completely.
            // this action costs 7 coins and can be blocked under specific conditions.
            // throws nothing on success, on failure will throw an excpetion
            void coup(Player &other);

            // the player chooses to end their turn
            // this action removes all actions they may have left as well as lifts any blocks on them
            void endTurn();

            // gets the name of the player
            string getName() const;

            // gets the number of coins the player has
            int coins() const;

            // gets the role of the player
            string getRole() const;

            // prints the player
            friend ostream& operator<<(ostream &out, const Player &player);
            
            // adds coins to the player
            void addCoins(int amount);

            // removes coins from the player (if amount is bigger than the number of coins the player has, sets coin count to 0)
            void removeCoins(int amount);   

            // sets the coup action as blocked
            void blockCoup();

            // sets the tax action as blocked
            void blockTax();
            
            // sets the gather action as blocked
            void blockGather();
            
            // sets the arrest action as blocked
            void blockArrest();

        private:
            // function used to turn this class into a virtual class
            virtual void virtualFunction() = 0;

        protected:

            // check if the player is out of actions
            bool isOutOfActions() const; 

            // check if the player provided is the last player arrested by this player
            bool isLastArrested(const string &otherName) const;

            // check if the player is blocked from using the coup action
            bool isCoupBlocked() const;

            // check if the player is blocked from using the tax action
            bool isTaxBlocked() const;

            // check if the player is blocked from using the gather action
            bool isGatherBlocked() const;

            // check if the player is blocked from using the arrest action
            bool isArrestBlocked() const;

            // sets the coup action as unblocked
            void unblockCoup();

            // sets the tax action as unblocked
            void unblockTax();

            // sets the gather action as unblocked
            void unblockGather();

            // sets the arrest action as unblocked
            void unblockArrest();

            // removes amount from the actions left for this player
            void removeActions(int amount);
    };
}