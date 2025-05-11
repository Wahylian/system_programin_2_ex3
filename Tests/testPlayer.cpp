#include "doctest.h"
#include "../PlayerFolder/Player.hpp"
#include "../PlayerFolder/Baron.hpp"
#include "../PlayerFolder/General.hpp"
#include "../PlayerFolder/Governor.hpp"
#include "../PlayerFolder/Judge.hpp"
#include "../PlayerFolder/Merchant.hpp"
#include "../PlayerFolder/Spy.hpp"

using namespace coup;

TEST_CASE("Test Player Basic Functionality") {
    Player *p = new Baron("Alice");

    // test basic player functions
    SUBCASE("Test Player Name") {
        CHECK(p->getName() == "Alice");
    }
    SUBCASE("Test Player Role") {
        CHECK(p->getRole() == "Baron");
    }
    SUBCASE("Test Player Coins") {
        CHECK(p->coins() == 0);
    }

    // deleting the player
    delete p;
}

TEST_CASE("Coin Management") {
    

    SUBCASE("Test Add Coins") {
        Player *p = new Spy("Bob");

        // check that the coins are added correctly
        CHECK(p->coins() == 0);
        p->addCoins(5);
        CHECK(p->coins() == 5);

        // deleting the player
        delete p;
    }

    SUBCASE("Test Remove Coins") {
        Player *p = new Spy("Bob");

        // adds the coins to the player
        p->addCoins(10);
        CHECK(p->coins() == 10);
        // removes 3 coins from the player
        p->removeCoins(3);
        CHECK(p->coins() == 7);

        // attempts to remove more coins than the player has
        p->removeCoins(10); 
        CHECK(p->coins() == 0); // should set their coin value to 0

        // deleting the player
        delete p;
    }

}

TEST_CASE("Test Base Player Functionality"){
    Player *p1 = new Merchant("John");
    Player *p2 = new Governor("Doe");

    // since merchant does not override any of the basic actions 
    // and none of the basic actions act differently to governor
    // we can test the basic actions of the player class with them

    SUBCASE("Test Player Gather"){

    }

    // deleting the players
    delete p1;
    delete p2;
}