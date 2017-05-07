#include "card.h"

/// Base cards must be named like this:
/// one_H.png (Heath) one_D.png (Diamond) one_S.png (Spade) one_C.png (Club)
QString BaseCardPaths[14] =
{
    "placeholder" // same for all colours
    "ace" // same for all colours
    "two" // same for all colours
    "three" // same for all colours
    "four" // same for all colours
    "five" // same for all colours
    "six" // same for all colours
    "seven" // same for all colours
    "eight" // same for all colours
    "nine" // same for all colours
    "ten" // same for all colours
    "jack" // same for all colours
    "queen" // same for all colours
    "king" // same for all colours
};

QString charBaseCardColor[4] =
{
    "_H",
    "_D",
    "_S",
    "_C"
};

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
