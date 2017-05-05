#include "game_board.h"

cGameBoard::cGameBoard()
{

}

void cGameBoard::RandomizeCards()
{
    for (int i = 0; i < 32; i++)
    {
        Pack[i] = i+1;
    }
    shuffle(Pack.begin(), Pack.end(), std::default_random_engine(time(0)));

}

void cGameBoard::FillGame()
{
    int currentID = 0;

    for (int packID = 0; packID < 7; packID++)
    {
        for (int pos = 0; pos < ((packID) + 1); currentID++, pos++)
        {
            CDlist[packID].push_back(Card(currentID));
        }
    }

}

void cGameBoard::GenerateNewGame()
{
    RandomizeCards();
    FillGame();
}

void cGameBoard::LoadGame(int slotID)
{

}

void cGameBoard::RemoveCards(int CDlist, int count)
{

}

void cGameBoard::AddCard(int CDlist, Card *card)
{

}

bool cGameBoard::CanBeMoved(int CDlistID, int cardPos)
{
    if (cardPos >= CDlist[CDlistID].size())
        return false;

    for (; cardPos < CDlist[CDlistID].size; cardPos++)
    {
        if (((Card)CDlist[CDlistID].at(cardPos)).IsHidden())
            return false;
    }

    return true;
}

bool cGameBoard::ShowCard(int CDlist, int cardPos)
{
    if (!CanBeShown(CDlistID, cardPos))
        return false;

    ((Card)CDlist[CDlistID].at(cardPos)).SetHidden(false);
    return true;
}

bool cGameBoard::CanBeShown(int CDlistID, int cardPos) const
{
    if (CDlist[CDlistID].size() == (cardPos - 1))
    {
        if (!((Card)CDlist[CDlistID].at(cardPos)).IsHidden())
            return true;
    }

    return false;
}



cGameBoard::~cGameBoard()
{

}