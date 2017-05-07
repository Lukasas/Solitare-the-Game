#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "card.h"
#include <algorithm>
#include <array>
#include <vector>
#include <random>
#include <time.h>

/**
 * @brief The cGameBoard class
 * Usage:
 * There must be instance of this object. Then there is a function GenerateNewGame
 * that fills everything that's needed for the game to start.
 *
 * Functions to operate the game:
 * PickPackCard() -> Picks a card from pack and puts it in PackCard
 * GetPackCard() -> Returns the Card pointer (PackCard)
 *
 *
 *
 */
class cGameBoard
{
private:
    /**
     * @brief CDlist
     * Card slots
     */
    std::vector<Card*> CDlist[7];

    void RandomizeCards();
    void FillGame();
    std::array<int, 52> WholePackOfCards;
    std::vector<Card*> Slots[4];

    int currentPackCardId;
    std::vector<Card*> Pack;

    Card * PackCard;

    bool ended;

    void (*f)();
    /**
     * @brief gameEndedCheck
     * Checks if the game ended
     */
    void gameEndedCheck();

    /**
     * @brief GetCardFromList
     * Gets a card from CDlist
     * @param CDlistID
     * @param cardPos
     * @return
     */
    Card * GetCardFromList(int CDlistID, int cardPos);

public:
    /**
     * @brief cGameBoard
     * This constructor must be always called
     */
    cGameBoard();

    /**
     * @brief GetPackCard
     * Gets the picked card from pack
     * @return
     * Card pointer to determine the card
     */
    Card * GetPackCard();

    /**
     * @brief PickPackCard
     * Selects the top card of pack
     */
    void PickPackCard();

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
    void RemoveCards(int CDlistID, int count);

    /**
     * @brief AddCard
     * Adds a specific card to list, mostly used for creating or loading a game
     * @param CDlist
     * @param card
     */
    void AddCard(int CDlistID, Card *card);

    /**
     * @brief CanBeMovedFromList
     * This function needs to be called when someone tries to move one card
     * @param CDlist
     * @param cardPos
     * @return
     * Return true if its possible
     */
    bool CanBeMovedFromList(int CDlist, int cardPos);


    /**
     * @brief ShowCard
     * Turns the card / shows it
     * @param CDlist
     * @param cardPos
     * @return
     * Return true if show action was successfull
     */
    bool ShowCard(int CDlist, int cardPos);

    /**
     * @brief CanBeShown
     * Tests if the selected card can be shown
     * @param CDlistID
     * @param cardPos
     * @return
     * Returns true if can, otherwise false
     */
    bool CanBeShown(int CDlistID, int cardPos);

    /**
     * @brief RemoveCardFromPack
     * @return
     */
    Card RemoveCardFromPack();

    /**
     * @brief SetEndGameNotify
     * Sets the end game function that'll be executed when the game ends
     * @param (*f)()
     * simple void name() {} function
     */
    void SetEndGameNotify(void (*f)());

    /**
     * @brief EmptyPack
     * Checks if the Pack of cards is empty
     * @return
     * True or False, depends..
     */
    bool EmptyPack() const;

    /**
     * @brief MoveCardToSlotFromPack
     * Moves a card from Pack to Slot
     * @param SlotID
     * Slot ID to move card in (0 - 3)
     * @return
     * Return true if the move was successfull
     */
    bool MoveCardToSlotFromPack(int SlotID);

    /**
     * @brief MoveCardToListFromPack
     * Moves a card from pack to list
     * @param CDlistID
     * List ID (0-7)
     * @return
     * Return true if the move was successfull
     */
    bool MoveCardToListFromPack(int CDlistID);

    /**
     * @brief MoveCardToSlotFromList
     * Moves a card from list to Slot
     * @param CDlistID
     * ID of list
     * @param cardID
     * Card position from top (0-n)
     * @param SlotID
     * Slot Position (0 - 3)
     * @return
     * Return true if the move was successfull
     */
    bool MoveCardToSlotFromList(int CDlistID, int cardID, int SlotID);

    /**
     * @brief MoveCardToListFromList
     * Moves a card through the battleground
     * @param CDlistID
     * ID of list the card is from
     * @param cardID
     * ID of card from top
     * @param ToCDlistID
     * ID of list to put the card
     * @param TocardID
     * ID of card that is moving (from top)
     * @return
     * Return true if the move was successfull
     */
    bool MoveCardToListFromList(int CDlistID, int cardID, int ToCDlistID, int TocardID);

    /**
     * @brief GetTopCardFromSlot
     * Gets the most top card from slot
     * @param SlotID
     * ID of slot (0-4)
     * @return
     * Returns the Card
     */
    Card * GetTopCardFromSlot(int SlotID);

    /**
     * @brief GetCardList
     * Gets the whole List of cards
     * @param CDlistID
     * ID of list (0-6)
     * @return
     */
    std::vector<Card*> GetCardList(int CDlistID);

    ~cGameBoard();


};

#endif // GAME_BOARD_H
