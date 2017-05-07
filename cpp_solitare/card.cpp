#include "card.h"



Card::Card(const Card &otherone)
{
    value = otherone.value;
    colour = otherone.colour;
}

Card::Card(Cards CardId, CardColour colour) : value(CardId), colour(colour)
{
    /*if (CardId >= 0 && CardId <= 13 )
        LoadQPixmap(BaseCardPaths[CardId] + charBaseCardColor[colour] + ".png");*/
}

Card::Card(int ID)
{
    value = static_cast<Cards>(((ID-1) % 13)+1);
    colour = static_cast<CardColour>(static_cast<int>(floor((ID - 1) / 13.0f)));
}

int Card::GetID()
{
    return value + (colour * 13);
}

/*
Card::Card(int value, QString CardPath) : value(value)
{
    drawable::LoadQPixmap(CardPath);
}
*/

Cards Card::iGetCardValue() const
{
    return value;
}

CardColour Card::iGetCardColour() const
{
    return colour;
}

int Card::Compare(const Card & other) const
{
    return other.iGetCardValue() - value;
}

bool Card::CompareColours(const Card & other) const
{
    if ((   colour < 2 && other.iGetCardColour() < 2) ||
            colour > 1 && other.iGetCardColour() > 1)
        return true;
    return false;

}

bool Card::CanBePlaced(const Card & OnThisCard) const
{
    if (Compare(OnThisCard) == 1 && !CompareColours(OnThisCard))
        return true;
    return false;
}

bool Card::IsHidden() const
{
    return hidden;
}

void Card::SetHidden(bool h)
{
    hidden = h;
}


/*
void Card::LoadQPixmap(QString path)
{
    drawable::LoadQPixmap("Cards/" + path);
}*/
