#include "card.h"



Card::Card(const Card &otherone)
{
    value = otherone.value;
    colour = otherone.colour;
    hidden = otherone.hidden;
    name = otherone.name;
}

void Card::SetCardValue(Cards value)
{
    this->value = value;
}

void Card::SetCardColour(CardColour colour)
{
    this->colour = colour;
}

Card::Card(Cards CardId, CardColour colour, bool hidden, QString name) : value(CardId), colour(colour), name(name)
{
    /*if (CardId >= 0 && CardId <= 13 )
        LoadQPixmap(BaseCardPaths[CardId] + charBaseCardColor[colour] + ".png");*/
}

Card::Card(int ID)
{
    value = static_cast<Cards>(((ID-1) % 13)+1);
    colour = static_cast<CardColour>(static_cast<int>(floor((ID - 1) / 13.0f)));
    name = itoa(GetID(), new char[10], 10);
}

void Card::SetName(QString name)
{
    this->name = name;
}

QString Card::GetName() const
{
    return name;
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

bool Card::Equal(const Card & other) const
{
    return (colour == other.colour && Compare(other) == 0);
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

bool Card::CanBeStored(const Card &OnThisCard) const
{
    if (Compare(OnThisCard) == -1 && colour == OnThisCard.iGetCardColour())
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

bool Card::IsKing() const
{
    return value == 13;
}

bool Card::IsAce() const
{
    return value == 1;
}

/*
void Card::LoadQPixmap(QString path)
{
    drawable::LoadQPixmap("Cards/" + path);
}*/
