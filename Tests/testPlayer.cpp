#include "testPlayer.hpp"
// since test player is just a wrapper of the actual player class it allows to test the player class on it
// and the result should be the same as with player but with the protected methods now accessible

TEST_CASE("Test Player Basic Functionality") {
    // creates a testPlayer
    TestPlayer p = TestPlayer("Alice");

    // test basic player functions
    SUBCASE("Test Player Name") {
        CHECK(p.getName() == "Alice");
    }
    SUBCASE("Test Player Role") {
        CHECK(p.getRole() == "testPlayer");
    }
    SUBCASE("Test Player Coins") {
        CHECK(p.coins() == 0);
    }
}

TEST_CASE("Coin Management") {
    SUBCASE("Test Add Coins") {
        TestPlayer p = TestPlayer("Bob");

        // check that the coins are added correctly
        CHECK(p.coins() == 0);
        CHECK_NOTHROW(p.addCoins(5));
        CHECK(p.coins() == 5);

        // attempts to add a negative amount of coins
        CHECK_THROWS(p.addCoins(-5));
        // attempts to add 0 coins
        CHECK_NOTHROW(p.addCoins(0));
        CHECK(p.coins() == 5);
    }

    SUBCASE("Test Remove Coins") {
        TestPlayer p = TestPlayer("Bob");

        // adds the coins to the player
        p.addCoins(10);
        CHECK(p.coins() == 10);
        // removes 3 coins from the player
        p.removeCoins(3);
        CHECK(p.coins() == 7);

        // attempts to remove more coins than the player has
        p.removeCoins(10); 
        CHECK(p.coins() == 0); // should set their coin value to 0
        
        // attempt to remove a negative amount of coins
        CHECK_THROWS(p.removeCoins(-5));
        // attempt to remove 0 coins
        CHECK_NOTHROW(p.removeCoins(0));
        CHECK(p.coins() == 0); // the amount shouldn't have changed
    }

}

TEST_CASE("Test Base Player Functionality"){

    SUBCASE("Test Player Gather"){
        SUBCASE("Test Successful Gather"){
            TestPlayer p1 = TestPlayer("John");
            p1.prepareForTurn(); // give the player an action
            CHECK(p1.getRemainingActions() == 1); // should be 1 action left
            CHECK(p1.coins() == 0); // should be 0
            CHECK_NOTHROW(p1.gather()); // should not throw an exception
            CHECK(p1.coins() == 1); // should have added 1 coin
            // check that the player has no actions left
            CHECK(p1.isOutOfActions());
        }

        SUBCASE("Test Player Gather Blocked"){
            TestPlayer p1 = TestPlayer("John");
            p1.prepareForTurn(); // give the player an action
            CHECK(p1.getRemainingActions() == 1); // should be 1 action left
            p1.blockGather(); // block thMerchante gather action
            CHECK_THROWS(p1.gather()); // should throw an exception
            // check that no action was performed
            CHECK(p1.getRemainingActions() == 1);
        }

        SUBCASE("Test Gather with No Actions Left"){
            TestPlayer p1 = TestPlayer("John");
            CHECK(p1.getRemainingActions() == 0); // should be 0 actions left
            CHECK_THROWS(p1.gather()); // should throw an exception
        } 
    }

    SUBCASE("Test Player Tax"){
        SUBCASE("Test Successful Tax"){
            TestPlayer p1 = TestPlayer("John");
            p1.prepareForTurn(); // give the player an action
            CHECK(p1.getRemainingActions() == 1); // should be 1 action left
            CHECK(p1.coins() == 0); // should be 0
            CHECK_NOTHROW(p1.tax()); // should not throw an exception
            CHECK(p1.coins() == 2); // should have added 2 coins
            // check that the player has no actions left
            CHECK(p1.getRemainingActions() == 0);
            
        }

        SUBCASE("Test Player Tax Blocked"){
            TestPlayer p1 = TestPlayer("John");
            p1.prepareForTurn(); // give the player an action
            // check that the player has 1 action left
            CHECK(p1.getRemainingActions() == 1);
            p1.blockTax(); // block the tax action
            CHECK_THROWS(p1.tax()); // should throw an exception
            // check that no action was performed
            CHECK(p1.getRemainingActions() == 1);
            
        }

        SUBCASE("Test Tax with No Actions Left"){
            TestPlayer p1 = TestPlayer("John");
            CHECK(p1.getRemainingActions() == 0); // should be 0 actions left
            CHECK_THROWS(p1.tax()); // should throw an exception
            
        }
    }

    SUBCASE("Test Player Bribe"){
        SUBCASE("Test Bribe Without Enough Coins"){
            TestPlayer p1 = TestPlayer("John");
            p1.prepareForTurn(); // give the player an action
            // check that the player has 1 action left
            CHECK(p1.getRemainingActions() == 1);
            CHECK(p1.coins() == 0); // should be 0
            CHECK_THROWS(p1.bribe()); // if a player attempts to bribe without enough coins, it should throw an exception
            // check that no action was performed
            CHECK(p1.getRemainingActions() == 1);
            
        }

        SUBCASE("Test Successful Bribe"){
            TestPlayer p1 = TestPlayer("John");
            p1.prepareForTurn(); // give the player an action
            CHECK(p1.getRemainingActions() == 1); // should be 1 action left
            p1.addCoins(4); // add 4 coins to the player
            CHECK(p1.coins() == 4); // should be 4
            // check that the player has 1 action left
            CHECK(p1.getRemainingActions() == 1); 
            CHECK_NOTHROW(p1.bribe()); // attempt to bribe
            CHECK(p1.coins() == 0); // should be 0
            // check that the player has 2 actions left
            CHECK(p1.getRemainingActions() == 2);
            
        }

        SUBCASE("Test Bribe with No Actions Left"){
            TestPlayer p1 = TestPlayer("John");
            p1.addCoins(4); // add 4 coins to the player
            CHECK(p1.coins() == 4); // should be 4
            CHECK(p1.getRemainingActions() == 0); // should be 0 actions left
            // since bribe does not require an action, it should not throw an exception
            CHECK_NOTHROW(p1.bribe()); // attempt to bribe
            // check that the player has 1 action left
            CHECK(p1.getRemainingActions() == 1);
            
        }
    }

    SUBCASE("Test Player Arrest"){
        SUBCASE("Test Successful Arrest"){
            SUBCASE("Test Arrest Player with Money"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                // give p1 an action
                p1.prepareForTurn(); 
                // check that p1 has an action
                CHECK(p1.getRemainingActions() == 1);

                // gives p2 some coins for p1 to take
                p2.addCoins(5); // add 5 coins to p2
                CHECK(p2.coins() == 5);

                // check that p1 has no coins
                CHECK(p1.coins() == 0); 

                // attempt to arrest a player
                CHECK_NOTHROW(p1.arrest(p2)); // should not throw an exception

                // check that p1 has no actions left
                CHECK(p1.getRemainingActions() == 0);
                
                // checks that p1 has 1 coin
                CHECK(p1.coins() == 1); 
                // checks that p2 has 4 coins
                CHECK(p2.coins() == 4); 


                
                
            }

            SUBCASE("Test Arrest Player with No Money"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                // give p1 an action
                p1.prepareForTurn();
                // check that p1 has an action
                CHECK(p1.getRemainingActions() == 1);
    
                // check that p1 has no coins
                CHECK(p1.coins() == 0); 
                // check that p2 has no coins
                CHECK(p2.coins() == 0);

                // attempt to arrest a player
                CHECK_NOTHROW(p1.arrest(p2)); // should not throw an exception

                // check that p1 has no actions left
                CHECK(p1.getRemainingActions() == 0);
                
                // checks that p1 has 0 coin
                CHECK(p1.coins() == 0); 
                // checks that p2 has 0 coins
                CHECK(p2.coins() == 0);
            }

            SUBCASE("Test Arrest 2 Different Players"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");
                TestPlayer p3 = TestPlayer("Smith");

                // give p1 an action
                p1.prepareForTurn();
                CHECK(p1.getRemainingActions() == 1); // should be 1 action left

                // give p2 and p3 some coins for p1 to take
                p2.addCoins(5); // add 5 coins to p2
                p3.addCoins(5); // add 5 coins to p3

                // check that p1 has no coins
                CHECK(p1.coins() == 0);
                // check that p2 has 5 coins
                CHECK(p2.coins() == 5);
                // check that p3 has 5 coins
                CHECK(p3.coins() == 5);

                // attempt to arrest p2
                CHECK_NOTHROW(p1.arrest(p2)); // should not throw an exception

                // check that p1 has no actions left
                CHECK(p1.getRemainingActions() == 0);

                // checks that p1 has 1 coin
                CHECK(p1.coins() == 1);
                // checks that p2 has 4 coins
                CHECK(p2.coins() == 4);

                // give an action to p1
                p1.prepareForTurn();
                CHECK(p1.getRemainingActions() == 1); // should be 1 action left

                // attempt to arrest p3
                CHECK_NOTHROW(p1.arrest(p3)); // should not throw an exception

                // check that p1 has no actions left
                CHECK(p1.getRemainingActions() == 0);

                // checks that p1 has 2 coins
                CHECK(p1.coins() == 2);
                // checks that p3 has 4 coins
                CHECK(p3.coins() == 4);
            }
        }

        SUBCASE("Test Player Arrest Failed"){
            SUBCASE("Test Arrest Self"){
                TestPlayer p1 = TestPlayer("John");
                p1.prepareForTurn(); // give the player an action
                CHECK(p1.getRemainingActions() == 1); // should be 1 action left
                CHECK_THROWS(p1.arrest(p1)); // should throw an exception
                // check that no action was performed
                CHECK(p1.getRemainingActions() == 1);
                
            }

            SUBCASE("Test Arrest Last Arrested Player"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                // give p1 an action
                p1.prepareForTurn(); // give the player an action
                CHECK(p1.getRemainingActions() == 1); // should be 1 action left

                // gives p2 some coins for p1 to take
                p2.addCoins(5); // add 5 coins to p2

                // attempt to arrest a player
                CHECK_NOTHROW(p1.arrest(p2)); // should not throw an exception

                // check that p1 has no actions left
                CHECK(p1.getRemainingActions() == 0);
                
                // checks that p1 has 1 coin
                CHECK(p1.coins() == 1); 
                // checks that p2 has 4 coins
                CHECK(p2.coins() == 4); 

                // attempt to arrest the last arrested player
                CHECK_THROWS(p1.arrest(p2)); // should throw an exception
            }

            SUBCASE("Test Arrest Blocked"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                // give p1 an action
                p1.prepareForTurn(); // give the player an action
                CHECK(p1.getRemainingActions() == 1); // should be 1 action left

                // block the arrest action
                p1.blockArrest();

                // attempt to arrest a player
                CHECK_THROWS(p1.arrest(p2)); // should throw an exception

                
                
            }

            SUBCASE("Test Arrest With No Actions Left"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                // check that p1 has no actions left
                CHECK(p1.getRemainingActions() == 0); 

                // attempt to arrest a player
                CHECK_THROWS(p1.arrest(p2)); // should throw an exception
                
                
            }
        }
    }  

    SUBCASE("Test Player Sanction"){
        SUBCASE("Test Successful Sanction"){
            TestPlayer p1 = TestPlayer("John");
            TestPlayer p2 = TestPlayer("Doe");

            // give p1 an action
            p1.prepareForTurn(); 
            CHECK(p1.getRemainingActions() == 1); // should be 1 action left

            // give p1 enough coins to perform the action
            p1.addCoins(3); // add 3 coins to p1

            // check that the financial actions in p2 are not blocked
            CHECK(p2.isTaxBlocked() == false);
            CHECK(p2.isGatherBlocked() == false);

            // sanction p2
            CHECK_NOTHROW(p1.sanction(p2)); // should not throw an exception

            // check that p1 has no actions left
            CHECK(p1.getRemainingActions() == 0);

            // check that p1 has 0 coins
            CHECK(p1.coins() == 0);

            // check that the financial actions in p2 are blocked
            CHECK(p2.isTaxBlocked() == true);
            CHECK(p2.isGatherBlocked() == true);
        }

        SUBCASE("Test Failed Sanction"){
            SUBCASE("Test Sanction Self"){
                TestPlayer p1 = TestPlayer("John");
                p1.prepareForTurn(); // give the player an action
                CHECK(p1.getRemainingActions() == 1); // should be 1 action left
                CHECK_THROWS(p1.sanction(p1)); // should throw an exception
                // check that no action was performed
                CHECK(p1.getRemainingActions() == 1);
                
            }

            SUBCASE("Test Sanction Without Enough Coins"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                p1.prepareForTurn(); // give the player an action
                CHECK(p1.getRemainingActions() == 1); // should be 1 action left

                // check that p1 has no coins
                CHECK(p1.coins() == 0); 

                // attempt to sanction a player
                CHECK_THROWS(p1.sanction(p2)); // should throw an exception

                
                
            }

            SUBCASE("Test Sanction Out of Actions"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                // check that p1 has no actions left
                CHECK(p1.getRemainingActions() == 0); 

                // attempt to sanction a player
                CHECK_THROWS(p1.sanction(p2)); // should throw an exception

                
                
            }
        }
    }

    SUBCASE("Test Player Coup"){
        SUBCASE("Test Successful Coup"){
            TestPlayer p1 = TestPlayer("John");
            TestPlayer p2 = TestPlayer("Doe");

            p1.prepareForTurn(); // give the player an action
            CHECK(p1.getRemainingActions() == 1); // should be 1 action left

            // give p1 enough coins to perform the coup
            p1.addCoins(7); // add 7 coins to p1
            CHECK(p1.coins() == 7); // should be 7 coins

            // check that p2 does not have a coup block on them
            CHECK(p2.isCoupBlocked() == false);

            // perform the coup
            CHECK_NOTHROW(p1.coup(p2)); // should not throw an exception
            // check that p1 has no actions left
            CHECK(p1.getRemainingActions() == 0);
            // check that p1 has 0 coins
            CHECK(p1.coins() == 0);
        }

        SUBCASE("Test Failed Coup"){
            SUBCASE("Test Coup Self"){
                TestPlayer p1 = TestPlayer("John");

                p1.prepareForTurn(); // give the player an action

                CHECK(p1.getRemainingActions() == 1); // should be 1 action left
                CHECK_THROWS(p1.coup(p1)); // should throw an exception

                // check that no action was performed
                CHECK(p1.getRemainingActions() == 1);
            }

            SUBCASE("Test Coup Without Enough Actions"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                // give p1 enough coins to perform the coup
                p1.addCoins(7); // add 7 coins to p1

                // check that p1 has no actions left
                CHECK(p1.getRemainingActions() == 0); 

                // attempt to coup a player
                CHECK_THROWS(p1.coup(p2)); // should throw an exception
            }

            SUBCASE("Test Coup Without Enough Coins"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                p1.prepareForTurn(); // give the player an action
                CHECK(p1.getRemainingActions() == 1); // should be 1 action left

                // check that p1 has no coins
                CHECK(p1.coins() == 0); 

                // attempt to coup a player
                CHECK_THROWS(p1.coup(p2)); // should throw an exception
            }

            SUBCASE("Test Coup Blocked"){
                TestPlayer p1 = TestPlayer("John");
                TestPlayer p2 = TestPlayer("Doe");

                // give p1 enough coins to perform the coup
                p1.addCoins(7); // add 7 coins to p1
                // give p1 an action
                p1.prepareForTurn();

                // block the coup action
                p2.blockCoup();

                // attempt to coup a player
                CHECK_THROWS(p1.coup(p2)); // should throw an exception
            }
        }
    }

    SUBCASE("Test Player End Turn"){
        SUBCASE("End Turn with Remaining Actions"){
            TestPlayer p = TestPlayer("John");

            // gives the player an action
            p.prepareForTurn();

            // check that they have an action
            CHECK(p.getRemainingActions() == 1);

            // end their turn
            CHECK_NOTHROW(p.endTurn());

            // check that they have no more actions
            CHECK(p.isOutOfActions());
        }

        SUBCASE("End Turn with No Remaining Actions"){
            TestPlayer p = TestPlayer("John");

            // check that the player has no actions
            CHECK(p.isOutOfActions());

            // ends their turn
            CHECK_NOTHROW(p.endTurn());

            //check that they still have no actiosn
            CHECK(p.isOutOfActions());
        }

        SUBCASE("End Turn with Blocked Actions"){
            SUBCASE("Coup Blocked"){
                TestPlayer p = TestPlayer("John");  

                // block coup on p
                p.blockCoup();

                // check that coup is blocked
                CHECK(p.isCoupBlocked());

                // end the player's turn
                CHECK_NOTHROW(p.endTurn());

                // check that coup is not blocked anymore
                CHECK(!p.isCoupBlocked());
            }
            SUBCASE("Gather Blocked"){
                TestPlayer p = TestPlayer("John");  
                
                // block gather on p
                p.blockGather();

                // check that gather is blocked
                CHECK(p.isGatherBlocked());

                // end the player's turn
                CHECK_NOTHROW(p.endTurn());

                // check that gather is not blocked anymore
                CHECK(!p.isGatherBlocked());
            }
            SUBCASE("Tax Blocked"){
                TestPlayer p = TestPlayer("John");   

                // block tax on p
                p.blockTax();

                // check that tax is blocked
                CHECK(p.isTaxBlocked());

                // end the player's turn
                CHECK_NOTHROW(p.endTurn());

                // check that tax is not blocked anymore
                CHECK(!p.isTaxBlocked());
            }
            SUBCASE("Arrest Blocked"){
                TestPlayer p = TestPlayer("John"); 
                
                // block arrest on p
                p.blockArrest();

                // check that arrest is blocked
                CHECK(p.isArrestBlocked());

                // end the player's turn
                CHECK_NOTHROW(p.endTurn());

                // check that arrest is not blocked anymore
                CHECK(!p.isArrestBlocked());
            }
            SUBCASE("Assortment Blocked"){
                TestPlayer p = TestPlayer("John");   

                // gives the player an action
                p.prepareForTurn();

                // give them multiple blocked actions
                p.blockTax();
                p.blockArrest();

                // check that said actions are blocked
                CHECK(p.isTaxBlocked());
                CHECK(p.isArrestBlocked());

                // end the players turn
                CHECK_NOTHROW(p.endTurn());

                // check that they dont have an action anymore
                CHECK(p.isOutOfActions());
                
                // check that the blocked actions are no longer blocked
                CHECK(!p.isTaxBlocked());
                CHECK(!p.isArrestBlocked());
            }
            SUBCASE("All Blocked"){
                TestPlayer p = TestPlayer("John"); 
                
                // blocks all the blockable actions and coup
                p.blockTax();
                p.blockGather();
                p.blockArrest();
                p.blockCoup();

                // check that they are all blocked
                CHECK(p.isTaxBlocked());
                CHECK(p.isGatherBlocked());
                CHECK(p.isArrestBlocked());
                CHECK(p.isCoupBlocked());

                // end the players turn
                CHECK_NOTHROW(p.endTurn());

                // check that they are now all unblocked
                CHECK(!p.isTaxBlocked());
                CHECK(!p.isGatherBlocked());
                CHECK(!p.isArrestBlocked());
                CHECK(!p.isCoupBlocked());
            }
        }
    }
}

TEST_CASE("Test Player Copy Constructor"){
    TestPlayer p1 = TestPlayer("John");
    
    // give p1 an action and some coins
    p1.prepareForTurn(); 
    p1.addCoins(5);

    TestPlayer p2 = TestPlayer(p1); // copy constructor

    // check that the players are the same
    CHECK(p1.getName() == p2.getName());
    CHECK(p1.getRole() == p2.getRole());
    CHECK(p1.coins() == p2.coins());
    CHECK(p1.getRemainingActions() == p2.getRemainingActions());

    // check that the players are not the same object
    CHECK(&p1 != &p2); // check that they are not the same object
}

TEST_CASE("Test Player Assignment Operator"){
    SUBCASE("Assignment to self"){
        TestPlayer p1 = TestPlayer("John");
        
        // give p1 an action and some coins
        p1.prepareForTurn(); 
        p1.addCoins(5);

        // save the original address of p1
        void *originalAddress = &p1;

        // pragmas to allow for self assignment
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wself-assign-overloaded"
        p1 = p1; // Self-assignment
        #pragma clang diagnostic pop

        // check that the players are the same
        CHECK(p1.getName() == "John");
        CHECK(p1.getRole() == "testPlayer");
        CHECK(p1.coins() == 5);
        CHECK(p1.getRemainingActions() == 1);

        // check that the players are the same object
        CHECK(originalAddress == &p1);
    }

    SUBCASE("Assignment to different player"){
        TestPlayer p1 = TestPlayer("John");
    
        // give p1 an action and some coins
        p1.prepareForTurn(); 
        p1.addCoins(5);

        TestPlayer p2 = TestPlayer("Doe"); // create a new player

        // check that the players are not the same
        CHECK(p1.getName() != p2.getName());
        CHECK(p1.getRole() == p2.getRole()); // since they are both test players
        CHECK(p1.coins() != p2.coins());
        CHECK(p1.getRemainingActions() != p2.getRemainingActions());

        // assign p1 to p2
        p2 = p1;

        // check that the players are the same
        CHECK(p1.getName() == p2.getName());
        CHECK(p1.getRole() == p2.getRole());
        CHECK(p1.coins() == p2.coins());
        CHECK(p1.getRemainingActions() == p2.getRemainingActions());

        // check that the players are not the same object
        CHECK(&p1 != &p2);
    }
}

TEST_CASE("Test Player Valgrind"){
    // test that the player is not causing any memory leaks
    SUBCASE("TestPlayer"){
        // create a player
        TestPlayer *p = new TestPlayer("John");

        // check that the player is not null
        CHECK(p != nullptr);

        // delete the player
        delete p;
    }

    SUBCASE("TestPlayer Copy Constructor"){
        // create a player
        TestPlayer *p1 = new TestPlayer("John");

        // check that the player is not null
        CHECK(p1 != nullptr);

        // create a copy of the player
        TestPlayer *p2 = new TestPlayer(*p1);

        // check that the copy is not null
        CHECK(p2 != nullptr);

        // delete the players
        delete p1;
        delete p2;
    }

    SUBCASE("TestPlayer Assignment Operator"){
        // create a player
        TestPlayer *p1 = new TestPlayer("John");

        // check that the player is not null
        CHECK(p1 != nullptr);

        // create a copy of the player
        TestPlayer *p2 = new TestPlayer("Doe");

        // check that the copy is not null
        CHECK(p2 != nullptr);

        // assign p1 to p2
        *p2 = *p1;

        // check that the players are the same
        CHECK(p1->getName() == p2->getName());
        CHECK(p1->getRole() == p2->getRole());
        CHECK(p1->coins() == p2->coins());
        CHECK(p1->getRemainingActions() == p2->getRemainingActions());

        // delete the players
        delete p1;
        delete p2;
    }

    SUBCASE("Testing other Player Types"){
        SUBCASE("Test Spy"){
            // create a spy
            Spy *p = new Spy("John");

            // check that the player is not null
            CHECK(p != nullptr);

            // delete the player
            delete p;
        }
        SUBCASE("Test Merchant"){
            // create a merchant
            Merchant *p = new Merchant("John");

            // check that the player is not null
            CHECK(p != nullptr);

            // delete the player
            delete p;
        }
        SUBCASE("Test Judge"){
            // create a judge
            Judge *p = new Judge("John");

            // check that the player is not null
            CHECK(p != nullptr);

            // delete the player
            delete p;
        }
        SUBCASE("Test Governor"){
            // create a governor
            Governor *p = new Governor("John");

            // check that the player is not null
            CHECK(p != nullptr);

            // delete the player
            delete p;
        }
        SUBCASE("Test General"){
            // create a general
            General *p = new General("John");

            // check that the player is not null
            CHECK(p != nullptr);

            // delete the player
            delete p;
        }
        SUBCASE("Test Baron"){
            // create a baron
            Baron *p = new Baron("John");
            // check that the player is not null
            CHECK(p != nullptr);    
            // delete the player
            delete p;
        }
    }
}
