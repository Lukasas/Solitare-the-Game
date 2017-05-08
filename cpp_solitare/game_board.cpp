/**
  File: game_board.cpp
  Author: Lukáš Chábek (xchabe00)
  Description: Handles the whole functionality of the deck. Loading / Saving / Creating new game and so. Has everything that's needed for making a deck.
  **/

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

    SaveCurrentState();

    if (PackCard != 0)
        currentPackCardId++;

    if (currentPackCardId >= Pack.size())
        currentPackCardId = 0;

    PackCard = Pack[currentPackCardId];
    return true;
}

void cGameBoard::RandomizeCards(int seed)
{
    for (int i = 0; i < 52; i++)
    {
        WholePackOfCards[i] = i+1;
    }
    shuffle(WholePackOfCards.begin(), WholePackOfCards.end(), std::default_random_engine(seed));

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
        c->SetHidden(false);
        Pack.push_back(c);
    }

}

void cGameBoard::SaveCurrentState()
{
    StepBack * backstep = new StepBack;
    for (int i = 0; i < 7; i++)
    {
        for (unsigned int j = 0; j < CDlist[i].size(); j++)
        {
            backstep->CDlist[i].push_back(new Card(*((Card*)CDlist[i].at(j))));
        }
        if (i < 4)
        {
            for (unsigned int j = 0; j < Slots[i].size(); j++)
            {
                backstep->Slots[i].push_back(new Card(*((Card*)Slots[i].at(j))));
            }
        }
    }

    backstep->currentPackCardId = currentPackCardId;
    backstep->Pack = Pack;
    backstep->PackCard = PackCard;

    if (back.size() == MAX_STEP_BACKS)
    {
        back.erase(back.begin());
    }
    back.push_back(backstep);
}

void cGameBoard::MakeStepBack()
{
    if (back.size() == 0)
        return;
    ClearGame();
    StepBack * backstep = back.back();
    for (int i = 0; i < 7; i++)
    {
        CDlist[i] = backstep->CDlist[i];
        if (i < 4)
        {
            Slots[i] = backstep->Slots[i];
        }
    }

    currentPackCardId = backstep->currentPackCardId;
    Pack = backstep->Pack;
    PackCard = backstep->PackCard;

    back.pop_back();
}

std::vector<Card *> cGameBoard::GetCardSlot(int SlotID)
{
    return Slots[SlotID];
}

bool cGameBoard::gameEndedCheck()
{
    for (int i = 0; i < 7; i++)
    {
        if (CDlist[i].size() != 0)
            return false;
    }

    if  (Pack.size() != 0)
        return false;

    if (PackCard != 0)
        return false;

    return true;
}

void cGameBoard::GenerateNewGame(int seed)
{
    ClearGame();

    RandomizeCards(seed);
    FillGame();
}

#define NEW_LINE_CUSTOM 100
#define NOCARD 101
bool cGameBoard::SaveGame(std::string slotID)
{
    if (slotID.length() < 6)
    {
        return false;
    }

    if (slotID.compare(slotID.length()-5, 5, ".game") != 0)
        slotID += ".game";

    FILE * f = fopen(slotID.c_str(), "wb");

    if (f == 0)
        return false;

    char saver;
    for (int i = 0; i < 7; i++)
    {
        for (unsigned int j = 0; j < CDlist[i].size(); j++)
        {
            saver = ((Card*)CDlist[i].at(j))->GetID();
            if (!fwrite((char*)&saver, 1, 1, f))
                throw "Could not write !";
            if (((Card*)CDlist[i].at(j))->IsHidden())
                saver = 0;
            else
                saver = 1;
            fwrite((char*)&saver, 1, 1, f);
        }
        saver = NEW_LINE_CUSTOM;
        if (!fwrite((char*)&saver, 1, 1, f))
            throw "Could not write !";
    }

    for (int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < Slots[i].size(); j++)
        {
            saver = ((Card*)Slots[i].at(j))->GetID();
            if (!fwrite((char*)&saver, 1, 1, f))
                throw "Could not write !";
            if (((Card*)Slots[i].at(j))->IsHidden())
                saver = 0;
            else
                saver = 1;
            if (!fwrite((char*)&saver, 1, 1, f))
                throw "Could not write !";
        }
        saver = NEW_LINE_CUSTOM;
        if (!fwrite((char*)&saver, 1, 1, f))
            throw "Could not write !";
    }

    saver = currentPackCardId;
    if (!fwrite((char*)&saver, 1, 1, f))
        throw "Could not write !";

    for (unsigned int i = 0; i < Pack.size(); i++)
    {
        saver = ((Card*)Pack.at(i))->GetID();
        if (!fwrite((char*)&saver, 1, 1, f))
            throw "Could not write !";
        if (((Card*)Pack.at(i))->IsHidden())
            saver = 0;
        else
            saver = 1;
        if (!fwrite((char*)&saver, 1, 1, f))
            throw "Could not write !";
    }
    saver = NEW_LINE_CUSTOM;
    if (!fwrite((char*)&saver, 1, 1, f))
        throw "Could not write !";

    if (PackCard)
    {
        saver = PackCard->GetID();
        if (!fwrite((char*)&saver, 1, 1, f))
            throw "Could not write !";
        if (PackCard->IsHidden())
            saver = 0;
        else
            saver = 1;
        if (!fwrite((char*)&saver, 1, 1, f))
            throw "Could not write !";
    }
    else
    {
        saver = NOCARD;
        if (!fwrite((char*)&saver, 1, 1, f))
            throw "Could not write !";
    }
    fclose(f);
    // Saved
    return true;
}

bool cGameBoard::LoadGame(std::string slotID)
{
    FILE *f = fopen(slotID.c_str(), "rb");
    if (f == NULL)
        return false;
    ClearGame();
    char loader;

    for (int i = 0; i < 7; i++)
    {
        while ((loader = fgetc(f)) != NEW_LINE_CUSTOM)
        {
            Card *c = new Card(loader);
            loader = fgetc(f); // hidden
            c->SetHidden(loader == 0);
            CDlist[i].push_back(c);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        while ((loader = fgetc(f)) != NEW_LINE_CUSTOM)
        {
            Card *c = new Card(loader);
            loader = fgetc(f); // hidden
            c->SetHidden(loader == 0);
            Slots[i].push_back(c);
        }
    }

    currentPackCardId = fgetc(f);

    while ((loader = fgetc(f)) != NEW_LINE_CUSTOM)
    {
        Card *c = new Card(loader);
        loader = fgetc(f); // hidden
        c->SetHidden(loader == 0);
        Pack.push_back(c);
    }

    loader = fgetc(f); // PackCard
    if (loader != NOCARD)
    {
        Card *c = new Card(loader);
        loader = fgetc(f); // hidden
        c->SetHidden(loader == 0);
        PackCard = c;
    }
    else
    {
        PackCard = 0;
    }
    fclose(f);
    // Loaded lol
    return true;
}


void cGameBoard::AddCard(int CDlistID, Card *card)
{
    CDlist[CDlistID].push_back(card);
}



bool cGameBoard::CanBeMovedFromList(int CDlistID, unsigned int cardPos)
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

bool cGameBoard::CanBeMovedToSlot(int SlotID, Card *c)
{
    if (Slots[SlotID].size() == 0)
    {
        return c->IsAce();
    }
    Card * laying = Slots[SlotID][Slots[SlotID].size() - 1];
    return c->CanBeStored(*laying);
}

bool cGameBoard::ShowCard(int CDlistID, int cardPos)
{
    if (!CanBeShown(CDlistID, cardPos))
        return false;

    GetCardFromList(CDlistID, cardPos)->SetHidden(false);
    return true;
}

bool cGameBoard::CanBeShown(int CDlistID, unsigned int cardPos)
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
        for (unsigned int pos = 0; pos < CDlist[packID].size(); pos++)
        {
            Card * find = (Card*)CDlist[packID][pos];
            if (c->GetName().compare(find->GetName()) == 0)
                return CardPos(pos, packID);
        }
    }

    return CardPos(-1, -1);
}

bool cGameBoard::EmptyPack() const
{
    return Pack.size() == 0;
}

bool cGameBoard::MoveCardToSlotFromPack(int SlotID)
{
    if (!CanBeMovedToSlot(SlotID, PackCard))
    {
        return false;
    }
    SaveCurrentState();
    Slots[SlotID].push_back(PackCard);
    PackCard = 0;
    gameEndedCheck();
    return true;
}

bool cGameBoard::MoveCardToListFromPack(int CDlistID)
{
    if (!CanBeMovedToList(CDlistID, PackCard))
    {
        return false;
    }
    SaveCurrentState();
    CDlist[CDlistID].push_back(PackCard);

    PackCard = 0;

    return true;
}

bool cGameBoard::MoCaToSlFrLi(int CDlistID, int cardID, int SlotID)
{
    if (!CanBeMovedFromList(CDlistID, cardID))
        return false;

    if (!CanBeMovedToSlot(SlotID, GetCardFromList(CDlistID, cardID)))
    {
        return false;
    }
    SaveCurrentState();
    Slots[SlotID].push_back(CDlist[CDlistID][cardID]);
    CDlist[CDlistID].erase(CDlist[CDlistID].begin() + cardID);
    gameEndedCheck();
    return true;
}

bool cGameBoard::MoveCardToSlotFromList(CardPos card, int SlotID)
{
    return MoCaToSlFrLi(card.ListID, card.TopPos, SlotID);
}

bool cGameBoard::MoCaToLiFrLi(int CDlistID, int cardID, int ToCDlistID)
{
    if (!CanBeMovedFromList(CDlistID, cardID))
        return false;

    if (!CanBeMovedToList(ToCDlistID, GetCardFromList(CDlistID, cardID)))
        return false;
    SaveCurrentState();
    for (unsigned int pos = cardID; pos < CDlist[CDlistID].size(); pos++)
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



bool cGameBoard::MoveCardToListFromSlot(int SlotID, int ListID)
{
    if (!CanBeMovedToList(ListID, GetTopCardFromSlot(SlotID)))
    {
        return false;
    }
    SaveCurrentState();
    CDlist[ListID].push_back(GetTopCardFromSlot(SlotID));
    Slots[SlotID].pop_back();

    return true;
}

std::vector<Card *> cGameBoard::FindChildrenOfCard(CardPos c)
{
    std::vector<Card *> childs;
    for (unsigned int pos = c.TopPos + 1; pos < CDlist[c.ListID].size(); pos++)
    {
        childs.push_back(CDlist[c.ListID][pos]);
    }
    return childs;
}

Card *cGameBoard::GetTopCardFromSlot(int SlotID)
{
    return Slots[SlotID][Slots[SlotID].size() - 1];
}

std::vector<Card *> cGameBoard::GetCardList(int CDlistID)
{
    return CDlist[CDlistID];
}

void cGameBoard::ShowCard(CardPos c)
{
    Card *ce = GetCardFromList(c.ListID, c.TopPos);
    if (ce->IsHidden())
    {
        SaveCurrentState();
        ce->SetHidden(false);
    }
}

void cGameBoard::ClearGame()
{
    for (int i = 0; i < 7; i++)
    {
        if (i < 4)
        {
            Slots[i].clear();
        }
        CDlist[i].clear();
    }
    currentPackCardId = 0;
    Pack.clear();
    PackCard = 0;
}

cGameBoard::~cGameBoard()
{

}
