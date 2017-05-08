#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "card.h"
#include <algorithm>
#include <array>
#include <vector>
#include <random>
#include <time.h>
#include <stdio.h>

struct CardPos
{
    CardPos(int top, int list)
    {
        TopPos = top;
        ListID = list;
    }

    int TopPos;
    int ListID;
};

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

    struct StepBack
    {
        std::vector<Card*> CDlist[7];
        std::vector<Card*> Slots[4];
        int currentPackCardId;
        std::vector<Card*> Pack;
        Card * PackCard;
    };

    /**
     * @brief CDlist
     * Card slots
     */
    std::vector<Card*> CDlist[7];
    std::vector<Card*> Slots[4];
    int currentPackCardId;
    std::vector<Card*> Pack;
    Card * PackCard;

    std::vector<StepBack*> back;

    void RandomizeCards(int seed);
    void FillGame();


    std::array<int, 52> WholePackOfCards;

public:

    /**
     * @brief SaveCurrentState
     * Saves states for stepback function
     */
    void SaveCurrentState();

    /**
     * @brief MakeStepBack
     * It's undo...
     */
    void MakeStepBack();

    /**
     * @brief GetCardSlot
     * Returns whole final slot
     * @param SlotID
     * Id of slot (0-3)
     * @return
     * Returns std::vector<Card*>
     */
    std::vector<Card*> GetCardSlot(int SlotID);

    /**
     * @brief gameEndedCheck
     * Checks if the game ended
     */
    bool gameEndedCheck();

    /**
     * @brief GetCardFromList
     * Gets a card from CDlist
     * @param CDlistID
     * @param cardPos
     * @return
     */
    Card * GetCardFromList(int CDlistID, int cardPos);
    /**
     * @brief GetCardLocation
     * Returns a structure filled with location of Card
     * @param c
     * Pointer to Card
     * @return
     * Returns zeros in structure if not found
     */
    CardPos GetCardLocation(Card * c) const;

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
     * @return
     * Returns true if card was picked successfully
     */
    bool PickPackCard();

    /**
     * @brief GenerateGame
     * Function that generates completly new game
     */
    void GenerateNewGame(int seed = (rand() * 1000000));

    /**
     * @brief SaveGame
     * Saves the game to selected name
     * @param slotID
     * Name of savefile
     * @return
     * Return true if save was successfull
     */
    bool SaveGame(std::string slotID);

    /**
     * @brief LoadGame
     * This function load a saved game
     * @param slotID
     * Name of game
     * @return
     * Return true if load was successfull
     */
    bool LoadGame(std::string slotID);

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
    bool CanBeMovedFromList(int CDlistID, int cardPos);

    bool CanBeMovedToList(int CDlistID, Card * c);

    bool CanBeMovedToSlot(int SlotID, Card * c);

    /**
     * @brief ShowCard
     * Turns the card / shows it
     * @param CDlist
     * @param cardPos
     * @return
     * Return true if show action was successfull
     */
    bool ShowCard(int CDlistID, int cardPos);

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
     * Removes card from pack - Card was picked and placed somewhere else
     */
    void RemoveCardFromPack();

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
     * Returns true if the move was successfull
     */
    bool MoveCardToSlotFromPack(int SlotID);

    /**
     * @brief MoveCardToListFromPack
     * Moves a card from pack to list
     * @param CDlistID
     * List ID (0-7)
     * @return
     * Returns true if the move was successfull
     */
    bool MoveCardToListFromPack(int CDlistID);

    /**
     * @brief MoCaToSlFrLi
     * Moves a card from list to Slot
     * @param CDlistID
     * ID of list
     * @param cardID
     * Card position from top (0-n)
     * @param SlotID
     * Slot Position (0 - 3)
     * @return
     * Returns true if the move was successfull
     */
    bool MoCaToSlFrLi(int CDlistID, int cardID, int SlotID);

    /**
     * @brief MoveCardToSlotFromList
     * Moves a card from list to Slot (Wrapper)
     * @param card
     * Card location
     * @param SlotID
     * Slot Position (0 - 3)
     * @return
     * Returns true if the move was successfull
     */
    bool MoveCardToSlotFromList(CardPos card, int SlotID);

    /**
     * @brief MoCaToLiFrLi
     * Moves a card through the battleground
     * @param CDlistID
     * ID of list the card is from
     * @param cardID
     * ID of card from top
     * @param ToCDlistID
     * ID of list to put the card
     * @return
     * Returns true if the move was successfull
     */
    bool MoCaToLiFrLi(int CDlistID, int cardID, int ToCDlistID);

    /**
     * @brief MoveCardToListFromList
     * Simplyfied moving card (Wrapper)
     * @param card
     * Pointer to card which is supposed to move
     * @param toList
     * ID of list
     * @return
     * Returns true if the move was successfull
     */
    bool MoveCardToListFromList(CardPos card, int toList);

    /**
     * @brief MoveCardToListFromSlot
     * Moves a card from the final slot to list
     * @param SlotID
     * ID of slot
     * @param ListID
     * ID of list
     * @return
     * Returns true if the move was sucessfull
     */
    bool MoveCardToListFromSlot(int SlotID,  int ListID);

    /**
     * @brief FindChildsOfCard
     * Returns every child of this card in list
     * @param c
     * Position of parent card
     * @return
     * List of cards
     */
    std::vector<Card*> FindChildrenOfCard(CardPos c);

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

    /**
     * @brief ShowCard
     * Shows card (unhide) at selected position in list
     * @param c
     * Card position in list
     * @param show
     * True to show / False to hide
     */
    void ShowCard(CardPos c);

    /**
     * @brief ClearGame
     * Clears all packs and remove all cards from the table
     */
    void ClearGame();

    ~cGameBoard();


};

#endif // GAME_BOARD_H
