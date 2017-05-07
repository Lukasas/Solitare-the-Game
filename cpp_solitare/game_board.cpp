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

bool cGameBoard::PickPackCard()
{
    if (PackCard == 0 && Pack.size() == 0)
    {
        gameEndedCheck();
        return false;
    }

    if (PackCard != 0)
        currentPackCardId++;

    if (currentPackCardId >= Pack.size())
        currentPackCardId = 0;

    PackCard = Pack[currentPackCardId];
    return true;
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

    int test = WholePackOfCards[currentID-1];

    for (; currentID < 52; currentID++)
    {
        Card * c = new Card(WholePackOfCards[currentID]);
        c->SetHidden(false);
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

bool cGameBoard::CanBeMovedToList(int CDlistID, Card *c)
{
    if (CDlist[CDlistID].size() == 0)
    {
        return c->IsKing();
    }
    Card * laying = CDlist[CDlistID][CDlist[CDlistID].size() - 1];
    return c->CanBePlaced(*laying);
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
    if ((CDlist[CDlistID].size() - 1) == cardPos)
    {
        if (GetCardFromList(CDlistID, cardPos)->IsHidden())
            return true;
    }

    return false;
}

void cGameBoard::RemoveCardFromPack()
{
    PackCard = 0;
    Pack.erase(Pack.begin() + currentPackCardId);
}

Card *cGameBoard::GetCardFromList(int CDlistID, int cardPos)
{
    return (Card*)CDlist[CDlistID].at(cardPos);
}


CardPos cGameBoard::GetCardLocation(Card *c) const
{
    for (int packID = 0; packID < 7; packID++)
    {
        for (int pos = 0; pos < CDlist[packID].size(); pos++)
        {
            Card * find = (Card*)CDlist[packID][pos];
            if (c->GetName().compare(find->GetName()) == 0)
                return CardPos(pos, packID);
        }
    }

    return CardPos(-1, -1);
}

void cGameBoard::SetEndGameNotify(void (*f)())
{
    this->f = f;
}

bool cGameBoard::EmptyPack() const
{
    return Pack.size() == 0;
}

bool cGameBoard::MoveCardToListFromPack(int CDlistID)
{
    if (!CanBeMovedToList(CDlistID, PackCard))
    {
        qDebug("Cant be moved to list");
        return false;
    }
    CDlist[CDlistID].push_back(PackCard);

    PackCard = 0;

    return true;
}

bool cGameBoard::MoCaToLiFrLi(int CDlistID, int cardID, int ToCDlistID)
{
    if (!CanBeMovedFromList(CDlistID, cardID))
    {
        qDebug("Cant be moved from list");
        return false;
    }

    if (!CanBeMovedToList(ToCDlistID, GetCardFromList(CDlistID, cardID)))
    {
        qDebug("Cant be moved to list");
        return false;
    }

    for (int pos = cardID; pos < CDlist[CDlistID].size(); pos++)
    {
        CDlist[ToCDlistID].push_back(CDlist[CDlistID][pos]);
    }

    CDlist[CDlistID].erase(CDlist[CDlistID].begin() + cardID, CDlist[CDlistID].end()); //.size() - cardID);
    return true;
}

bool cGameBoard::MoveCardToListFromList(CardPos card, int toList)
{
    return MoCaToLiFrLi(card.ListID, card.TopPos, toList);
}

std::vector<Card *> cGameBoard::FindChildrenOfCard(CardPos c)
{
    std::vector<Card *> childs;
    for (int pos = c.TopPos + 1; pos < CDlist[c.ListID].size(); pos++)
    {
        childs.push_back(CDlist[c.ListID][pos]);
    }
    return childs;
}

std::vector<Card *> cGameBoard::GetCardList(int CDlistID)
{
    return CDlist[CDlistID];
}

void cGameBoard::ShowCard(CardPos c, bool show)
{
    GetCardFromList(c.ListID, c.TopPos)->SetHidden(!show);
}

cGameBoard::~cGameBoard()
{

}
