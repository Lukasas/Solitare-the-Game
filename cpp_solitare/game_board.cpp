#include "game_board.h"

cGameBoard::cGameBoard()
{
    PackCard = 0;
    currentPackCardId = 0;
}

Card *cGameBoard::GetPackCard()
{
    return PackCard;
}

void cGameBoard::PickPackCard()
{
    if (PackCard == 0 && Pack.size() == 0)
    {
        gameEndedCheck();
        return;
    }

    if (PackCard != 0)
        currentPackCardId++;

    if (currentPackCardId >= Pack.size())
        currentPackCardId = 0;

    PackCard = Pack[currentPackCardId];
}

void cGameBoard::RandomizeCards()
{
    for (int i = 0; i < 52; i++)
    {
        WholePackOfCards[i] = i+1;
    }
    shuffle(WholePackOfCards.begin(), WholePackOfCards.end(), std::default_random_engine(time(0)));

}

void cGameBoard::FillGame()
{
    int currentID = 0;

    for (int packID = 0; packID < 7; packID++)
    {
        for (int pos = 0; pos < ((packID) + 1); pos++)
        {
            Card * c = new Card(WholePackOfCards[currentID]);
            c->SetHidden(true);
            CDlist[packID].push_back(c);
            currentID++;
            if (pos == packID)
                c->SetHidden(false);
        }        
    }

    for (; currentID < 52; currentID++)
    {
        Card * c = new Card(WholePackOfCards[currentID]);
        Pack.push_back(c);
    }

}

void cGameBoard::gameEndedCheck()
{
    for (int i = 0; i < 7; i++)
    {
        if (CDlist[i].size() != 0)
            return;
    }

    if  (Pack.size() != 0)
        return;

    if (PackCard != 0)
        return;

    ended = true;

    f();
}

void cGameBoard::GenerateNewGame()
{
    RandomizeCards();
    FillGame();
}

void cGameBoard::LoadGame(int slotID)
{

}

void cGameBoard::RemoveCards(int CDlistID, int count)
{

}

void cGameBoard::AddCard(int CDlistID, Card *card)
{
    CDlist[CDlistID].push_back(card);
}



bool cGameBoard::CanBeMovedFromList(int CDlistID, int cardPos)
{
    if (cardPos >= CDlist[CDlistID].size())
        return false;



    for (; cardPos < CDlist[CDlistID].size(); cardPos++)
    {
        if (GetCardFromList(CDlistID, cardPos)->IsHidden())
            return false;
    }

    return true;
}

bool cGameBoard::ShowCard(int CDlistID, int cardPos)
{
    if (!CanBeShown(CDlistID, cardPos))
        return false;

    GetCardFromList(CDlistID, cardPos)->SetHidden(false);
    return true;
}

bool cGameBoard::CanBeShown(int CDlistID, int cardPos)
{
    if (CDlist[CDlistID].size() == (cardPos - 1))
    {
        if (!GetCardFromList(CDlistID, cardPos)->IsHidden())
            return true;
    }

    return false;
}

Card *cGameBoard::GetCardFromList(int CDlistID, int cardPos)
{
    return (Card*)CDlist[CDlistID].at(cardPos);
}

void cGameBoard::SetEndGameNotify(void (*f)())
{
    this->f = f;
}

bool cGameBoard::EmptyPack() const
{
    return Pack.size() == 0;
}

std::vector<Card *> cGameBoard::GetCardList(int CDlistID)
{
    return CDlist[CDlistID];
}

cGameBoard::~cGameBoard()
{

}
