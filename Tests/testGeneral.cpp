#include "testGeneral.hpp"

TEST_CASE("Test General Construction"){
    SUBCASE("Test General Constructor"){
        General g = General("John");
        
        // check that the values are correct
        CHECK(g.getName() == "John");
        CHECK(g.getRole() == "General");
        CHECK(g.coins() == 0);
        CHECK(g.getRemainingActions() == 0);
        CHECK(g.getLastArrested() == nullptr);
    }
    SUBCASE("Test General Copy Constructor"){
        General g1 = General("John");
        // give g1 some coins
        g1.prepareForTurn();
        g1.gather();
        g1.prepareForTurn();
        g1.gather();

        // create a copy of g1
        General g2 = General(g1);

        // check that the values are correct
        CHECK(g2.getName() == g1.getName());
        CHECK(g2.getRole() == g1.getRole());
        CHECK(g2.coins() == g1.coins());
        CHECK(g2.getRemainingActions() == g1.getRemainingActions());
        CHECK(g2.getLastArrested() == g1.getLastArrested());

        // check that the players are not the same object
        CHECK(&g1 != &g2);
    }
    SUBCASE("Test General Assignment Operator"){
        SUBCASE("Assignment to Self"){
            General g1 = General("John");
            // give g1 some coins
            g1.prepareForTurn();
            g1.gather();
            g1.prepareForTurn();
            g1.gather();

            // save the address of g1
            void *pG1 = &g1;

            // pragmas to allow for self assignment
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wself-assign-overloaded"
            g1 = g1; // Self-assignment
            #pragma clang diagnostic pop

            // check that the values are correct
            CHECK(g1.getName() == "John");
            CHECK(g1.getRole() == "General");
            CHECK(g1.coins() == 2);
            CHECK(g1.getRemainingActions() == 0);
            CHECK(g1.getLastArrested() == nullptr);
        
            // check that the player is saved in the same address
            CHECK(pG1 == &g1);
        }
        SUBCASE("Assignment to General"){
            General g1 = General("John");
            // give g1 some coins
            g1.prepareForTurn();
            g1.gather();
            g1.prepareForTurn();
            g1.gather();

            // assign g1 to g2
            General g2 = g1;

            // check that the values are correct
            CHECK(g2.getName() == g1.getName());
            CHECK(g2.getRole() == g1.getRole());
            CHECK(g2.coins() == g1.coins());
            CHECK(g2.getRemainingActions() == g1.getRemainingActions());
            CHECK(g2.getLastArrested() == g1.getLastArrested());

            // check that the players are not the same object
            CHECK(&g1 != &g2);
        }
    }
    SUBCASE("Test General Valid Actions"){
        SUBCASE("Test Valid Actions"){
            General g = General("John");

            // create a list of the excpected valid actions of the general
            vector<string> expectedValidActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup", "endTurn", "undoCoup"};
            
            // get the valid actions for the player
            vector<string> actualValidActions = g.getValidActions();

            // check that the vectors are the same
            CHECK(expectedValidActions == actualValidActions);

            // check that the general accepts the valid actions
            for(const string &action : expectedValidActions){
                CHECK(g.isValidAction(action) == true);
            }

            // check that the general does not accept actions of any other type of player
            CHECK(!g.isValidAction("spyOn") == true); // spy special action
            CHECK(!g.isValidAction("undoBribe") == true); // judge special action
            CHECK(!g.isValidAction("undoTax") == true); // governor special action
            CHECK(!g.isValidAction("blockArrest") == true); // spy special action

            // check that the general does not accept any other actions
            CHECK(!g.isValidAction("test") == true);
            CHECK(!g.isValidAction("yatzee") == true);
            CHECK(!g.isValidAction("banana") == true);
        }
        SUBCASE("Test Valid Undo Actions"){
            General g = General("John");

            // check that the general accepts only the undoCoup action
            CHECK(g.isValidUndoAction("undoCoup") == true);
            CHECK(!g.isValidUndoAction("undoBribe") == true); // judge special action
            CHECK(!g.isValidUndoAction("undoTax") == true); // governor special action
            // check that the baron does not accept any other actions as undo actions
            CHECK(!g.isValidUndoAction("test") == true);
            CHECK(!g.isValidUndoAction("yatzee") == true);
        }
    }
}

TEST_CASE("Test General Valgrind"){
    General *g = new General("John");
    // do multiple things with the general
    g->prepareForTurn();
    g->gather();
    g->prepareForTurn();
    g->gather();
    g->prepareForTurn();

    // check that the general has 2 coins
    CHECK(g->coins() == 2);
    // check that the general has 1 action left
    CHECK(g->getRemainingActions() == 1);
    // check that the general has not arrested anyone
    CHECK(g->getLastArrested() == nullptr);

    // delete the general
    delete g;
}

TEST_CASE("Test General Special Interactions"){
    SUBCASE("Test Undo Coup"){
        // since undo coup is a special action and is situational, it will be tested mainly in the game tests
        SUBCASE("Test Undo Coup on Self"){
            General g = General("John");
            // give g some coins
            g.prepareForTurn();
            g.gather();
            g.prepareForTurn();
            g.gather();

            // check that the general has 2 coins
            CHECK(g.coins() == 2);

            // attempt to block coup
            g.prepareForTurn();
            CHECK_NOTHROW(g.undoCoup(g)); // should not throw an exception

            // check that the general has 2 coins
            CHECK(g.coins() == 2);

            // check that the general has 1 actions left
            CHECK(g.getRemainingActions() == 1);
        }
        SUBCASE("Test Block Coup on Other"){
            General g1 = General("John");

            // give g1 an action
            g1.prepareForTurn();
            // check that g1 has no coins
            CHECK(g1.coins() == 0);

            General g2 = General("Doe");

            // attempt to block coup
            CHECK_NOTHROW(g1.undoCoup(g2)); // should not throw an exception

            // check that g1 still has an action and no coins
            CHECK(g1.getRemainingActions() == 1);
            CHECK(g1.coins() == 0);
        }
    }
    SUBCASE("Test Arrest Passive Interaction"){
        General g1 = General("John");
        // give g1 an action
        g1.prepareForTurn();
        // check that g1 has no coins
        CHECK(g1.coins() == 0);

        General g2 = General("Doe");
        // give g2 some coins
        g2.prepareForTurn();
        g2.tax();

        // attempt to arrest a player
        CHECK_NOTHROW(g1.arrest(g2)); // should not throw an exception

        // check that g1 has no actions left
        CHECK(g1.getRemainingActions() == 0);

        // check that g1 still has no coins 
        CHECK(g1.coins() == 0);
        // check that g2 has 2 coins
        CHECK(g2.coins() == 2);
    }
}