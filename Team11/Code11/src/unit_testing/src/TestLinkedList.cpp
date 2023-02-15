//
// Created by Toh Yu Ting on 9 Sept 2021
//

#include "DE/LinkedList.h"

#include "catch.hpp"

// Consider adding a test stub?

TEST_CASE("Check exists") {
    LinkedList* newList = new LinkedList();
    newList->insertHead(5);

    REQUIRE(newList->exist(5));
    REQUIRE(newList->exist(6) == false);
}

TEST_CASE("Get size") {
    LinkedList* newList = new LinkedList();
    SECTION("Empty List") {
        REQUIRE(newList->getSize() == 0);
    }
    newList->insertHead(1);
    newList->insertHead(2);
    newList->insertHead(3);
    newList->insertHead(4);
    newList->insertHead(5);
    // [5,4,3,2,1]
    SECTION("Filled List") {
        REQUIRE(newList->getSize() == 5);
    }
}

TEST_CASE("Check empty") {
    LinkedList* newList = new LinkedList();
    SECTION("Empty List") {
        REQUIRE(newList->empty() == 1);
    }
    newList->insertHead(1);
    newList->insertHead(2);
    newList->insertHead(3);
    newList->insertHead(4);
    newList->insertHead(5);
    // [5,4,3,2,1]
    SECTION("Filled List") {
        REQUIRE(newList->empty() == 0);
    }
}


TEST_CASE("Get Head Item") {
    LinkedList* newList = new LinkedList();
    SECTION("Empty List") {
        REQUIRE(newList->getHeadItem() == -1);
    }
    newList->insertHead(5);
    SECTION("Get Head Item") {
        REQUIRE(newList->getHeadItem() == 5);
    }
    newList->insertHead(6);
    SECTION("Get Head Item after adding") {
        REQUIRE(newList->getHeadItem() == 6);
    }
}

TEST_CASE("Get Tail Item") {
    LinkedList* newList = new LinkedList();
    SECTION("Empty List") {
        REQUIRE(newList->getTailItem() == -1);
    }
    newList->insertTail(5);
    SECTION("Get Tail Item") {
        REQUIRE(newList->getTailItem() == 5);
    }
    newList->insertTail(6);
    SECTION("Get Head Item after adding") {
        REQUIRE(newList->getTailItem() == 6);
    }
}

TEST_CASE("Get Item at Index") {
    LinkedList* newList = new LinkedList();
    SECTION("Empty List, invalid index") {
        REQUIRE(newList->getElementAtIndex(0) == -1);
    }
    newList->insertTail(5);
    SECTION("Empty List, valid index") {
        REQUIRE(newList->getElementAtIndex(0) == 5);
    }

    // add five items into the list
    newList->insertHead(1);
    newList->insertHead(2);
    newList->insertHead(3);
    newList->insertHead(4);
    newList->insertHead(5);
    // [5,4,3,2,1]

    SECTION("To filled List, valid") {
        REQUIRE(newList->getElementAtIndex(3) == 2);
    }

    SECTION("To filled List, invalid") {
        REQUIRE(newList->getElementAtIndex(80) == -1);
    }
}

TEST_CASE("Find element's index") {
    LinkedList* newList = new LinkedList();
    SECTION("Empty List") {
        REQUIRE(newList->findElementPos(4) == -1);
    }
    newList->insertHead(1);
    newList->insertHead(2);
    newList->insertHead(3);
    newList->insertHead(4);
    newList->insertHead(5);
    // [5,4,3,2,1]
    SECTION("Element not in list") {
        REQUIRE(newList->findElementPos(600) == -1);
    }

    SECTION("Element in list") {
        REQUIRE(newList->findElementPos(1) == 4);
        REQUIRE(newList->findElementPos(5) == 0);
        REQUIRE(newList->findElementPos(4) == 1);
    }
}

// Consider adding a test stub?
TEST_CASE("Insert to LinkedList") {
    LinkedList* newList = new LinkedList();
    SECTION("Insert via Head") {
        SECTION("To empty List") {
            newList->insertHead(5);
            REQUIRE(newList->exist(5));
            REQUIRE(newList->getHeadItem() == 5);
        }
        newList->insertHead(5);
        SECTION("To filled List") {
            newList->insertHead(4);
            REQUIRE(newList->exist(4));
            REQUIRE(newList->getHeadItem() == 4);
        }
    }

    SECTION("Insert via Tail") {
        SECTION("To empty List") {
            newList->insertTail(5);
            REQUIRE(newList->exist(5));
            REQUIRE(newList->getTailItem() == 5);
            REQUIRE(newList->getHeadItem() == 5);
        }
        newList->insertHead(5);
        SECTION("To filled List") {
            newList->insertTail(4);
            REQUIRE(newList->exist(4));
            REQUIRE(newList->getTailItem() == 4);
        }
    }

    SECTION("Insert at Position") {
        SECTION("To empty List, invalid position") {
            REQUIRE(newList->insertAtPos(5, 10) == false);
        }

        SECTION("To empty List, valid position") {
            REQUIRE(newList->insertAtPos(100, 0));
            REQUIRE(newList->getElementAtIndex(0) == 100);
        }

        // add five items into the list
        newList->insertHead(1);
        newList->insertHead(2);
        newList->insertHead(3);
        newList->insertHead(4);
        newList->insertHead(5);
        // [5,4,3,2,1]

        SECTION("To filled List, valid") {
            REQUIRE(newList->insertAtPos(900, 2));
            REQUIRE(newList->getElementAtIndex(2) ==  900);
        }

        SECTION("To filled List, invalid") {
            REQUIRE(newList->insertAtPos(900, 20) == false);
        }

    }

    SECTION("Insert after Position") {
        SECTION("To empty List, invalid position") {
            REQUIRE(newList->insertAtAfter(100,9) == false);
        }

        SECTION("To empty List, valid position") {
            REQUIRE(newList->insertAtAfter(100, 0));
            REQUIRE(newList->getElementAtIndex(0) == 100);
        }

        // add five items into the list
        newList->insertHead(1);
        newList->insertHead(2);
        newList->insertHead(3);
        newList->insertHead(4);
        newList->insertHead(5);
        // [5,4,3,2,1]

        SECTION("To filled List, valid") {
            REQUIRE(newList->insertAtAfter(100, 2));
            REQUIRE(newList->getElementAtIndex(3) == 100);
        }

        SECTION("To filled List, invalid") {
            REQUIRE(newList->insertAtAfter(900, 20) == false);
        }
    }

}

TEST_CASE("Remove Items") {
    LinkedList* newList = new LinkedList();
    SECTION("Empty list remove head") {
        newList->removeHead();
        REQUIRE(newList->getHeadItem() == -1);
    }

    SECTION("Empty list remove tail") {
        newList->removeTail();
        REQUIRE(newList->getTailItem() == -1);
    }

    SECTION("Empty list remove at position") {
        newList->removePos(8);
        REQUIRE(newList->getHeadItem() == -1);
    }

    SECTION("Empty list remove item") {
        newList->removeItem(10);
        REQUIRE(newList->getHeadItem() == -1);
    }

    // add five items into the list
    newList->insertHead(1);
    newList->insertHead(2);
    newList->insertHead(3);
    newList->insertHead(4);
    newList->insertHead(5);
    // [5,4,3,2,1]

    SECTION("List remove head") {
        newList->removeHead();
        REQUIRE(newList->getHeadItem() == 4);
    }

    SECTION("List remove tail") {
        newList->removeTail();
        REQUIRE(newList->getTailItem() == 2);
    }

    SECTION("List remove at invalid position") {
        newList->removePos(8);
        REQUIRE(newList->getTailItem() == 1);
    }

    SECTION("List remove at valid position") {
        newList->removePos(3);
        REQUIRE(newList->getElementAtIndex(3) == 1);
    }

    SECTION("List remove valid item") {
        REQUIRE(newList->removeItem(4) == true);
    }

    SECTION("List remove invalid item") {
        REQUIRE(newList->removeItem(100) == false);
    }

}

