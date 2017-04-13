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

Card::Card(Cards CardId, CardColour colour) : value(CardId), colour(colour)
{
    if (CardId >= 0 && CardId <= 13 )
        LoadQPixmap(BaseCardPaths[CardId] + charBaseCardColor[colour] + ".png");
}

Card::Card(int value, QString CardPath) : value(value)
{
    drawable::LoadQPixmap(CardPath);
}

Cards Card::iGetCardValue()
{
    return value;
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

void Card::LoadQPixmap(QString path)
{
    drawable::LoadQPixmap("Cards/" + path);
}
