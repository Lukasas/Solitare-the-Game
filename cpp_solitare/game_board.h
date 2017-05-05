#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "card.h"
#include <algorithm>
#include <array>
#include <vector>
#include <random>
#include <time.h>

class cGameBoard
{
private:
    /**
     * @brief CDqueues
     * Card slots
     */
    std::vector<Card> CDlist[7];

    void RandomizeCards();
    void FillGame();
    std::array<int, 52> Pack;
public:
    /**
     * @brief cGameBoard
     * This constructor must be always called
     */
    cGameBoard();

    /**
     * @brief GenerateGame
     * Function that generates completly new game
     */
    void GenerateNewGame();

    /**
     * @brief LoadGame
     * This function load a saved game
     * @param slotID
     * ID of save game (1-4)
     */
    void LoadGame(int slotID);

    /**
     * @brief RemoveCards
     * Removes cards from List, this is used for like moving cards
     * @param CDlist
     * ID of list
     * @param count
     * How many cards needs to be removed
     */
    void RemoveCards(int CDlist, int count);

    /**
     * @brief AddCard
     * Adds a specific card to list, mostly used for creating or loading a game
     * @param CDlist
     * @param card
     */
    void AddCard(int CDlist, Card * card);

    bool CanBeMoved(int CDlist, int cardPos);

    ~cGameBoard();

};

#endif // GAME_BOARD_H
