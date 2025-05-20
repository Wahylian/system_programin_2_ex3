#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "Game.hpp"
using namespace std;
using namespace coup;


void printPlayers(const vector<string> &players){
    for(int i = 0; i < players.size(); i++){
        string playerName = players[i];
        cout << i << ". " << playerName << " " << endl;
    }
}

int main(){
    // create a game with 6 players
    Game game = Game({"Alice", "Bob", "Charlie", "Dave", "Eve", "Frank"});
    // print the names of all the players
    cout << "Players in the game: ";
    printPlayers(game.players());


    // starts to run game
    while(true){
        // if the game is over, announce the winner and exit the loop
        try{
            string winnerName = game.winner();
            cout << "The winner is: " << winnerName << endl;
            break;
        }
        // if it is not over, continue the game
        catch(game_not_over_exception &e){
            // gets the name of the current player
            string currentPlayerName = game.turn();

            // prints the current player
            cout << "Current player: " << currentPlayerName << endl;
            
            while(true){
                // gets the action the player wants to perform
                string action;
                cout << "Enter action: ";
                cin >> action;

                // gets the target player for the action
                int targetPlayerIndex;
                cout << "Enter target player: ";
                printPlayers(game.players());
                cin >> targetPlayerIndex;
                while(targetPlayerIndex < 0 || targetPlayerIndex >= game.playersCount()){
                    cout << "Invalid player index" << endl;
                    cin >> targetPlayerIndex;
                }
                Player *targetPlayer = game.getPlayerByIndex(targetPlayerIndex);

                // attempt to perform the action
                try{
                    // if the turn is over, break the loop
                    if(game.playAction(action, *targetPlayer)){
                        break;
                    }

                    // check if the action provided is an undo-able action
                    if(action == "bribe"){
                        // gets all judges in the game that are not the current player
                        vector<Player*> judges = game.allOfRole("Judge");
                        for(const Player *judge : judges){
                            // ask the player if they want to undo the bribe
                            cout << judge->getName() <<" would you want to undo the bribe for ";
                            cout << currentPlayerName << "? (y/n): ";
                            string ans;
                            cin >> ans;
                            if(ans == "y"){
                                // undo the bribe
                                game.undoAction("Bribe");
                                cout << "Bribe undone for " << currentPlayerName << endl;
                                // breaks the loop
                                break;
                            }
                        }
                    }
                    if(action == "tax"){
                        // gets all the governors in the game that are not the current player
                        vector<Player*> governors = game.allOfRole("Governor");
                        for(const Player *governor : governors){
                            // ask the player if they want to undo the tax
                            cout << governor->getName() << " would you want to undo the tax for ";
                            cout << currentPlayerName << "? (y/n): ";
                            string ans;
                            cin >> ans;
                            if(ans == "y"){
                                // undos the taxation
                                game.undoAction("Tax");
                                cout << "Tax blocked for " << currentPlayerName << endl;
                                // breaks the loop
                                break;
                            }
                        }
                    }
                }
                // the action can fail for multiple reasons
                catch(invalid_action_exception &e){
                    cout << e.what() << endl;
                }
                catch(player_not_in_game_exception &e){
                    cout << e.what() << endl;
                }
                catch(out_of_actions_exception &e){
                    cout << e.what() << endl;
                }
                catch(blocked_action_exception &e){
                    cout << e.what() << endl;
                }
                catch(not_enough_coins_exception &e){
                    cout << e.what() << endl;
                }
                catch(double_jeopardy_exception &e){
                    cout << e.what() << endl;
                }
                catch(illegal_action_on_self_exception &e){
                    cout << e.what() << endl;
                }
            }
        }
    }

    

    return 0;
}