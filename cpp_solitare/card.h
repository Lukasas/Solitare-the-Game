#ifndef CARD_H
#define CARD_H

#include "drawable.h"

enum Cards
{
    CACE,    C1,    C2,
    C3,    C4,    C5,
    C6,    C7,    C8,
    C9,    C10,    CJ,
    CQ,    CK
};


enum CardColour
{
    Heart, // red
    Diamond, // red
    Spade, // black
    Club // black
};

extern QString *charBaseCardColor;
extern QString *BaseCardPaths;

class Card : public drawable
{
private:
    Cards value;
    CardColour colour;

    /**
     * @brief LoadQPixmap
     *      Overrides base function, extends the functionality of adding right base Folder
     *      This method is private becouse only constructor can Load card texture.
     * @param path
     */
    void LoadQPixmap(QString path) override;
public:
    explicit Card(Cards CardId, CardColour colour);
    explicit Card(int value, QString CardPath);
    /**
     * @brief iGetCardValue
     *      Returns card value (1>K)
     * @return
     *      Returns an integer depenting on selected Card
     *      0 1 2 3 4 5 6 7 8 9 10 11 12 13
     *      A 1 2 3 4 5 6 7 8 9 10  J  Q  K
     */
    Cards iGetCardValue() const;
    /**
     * @brief iGetCardColour
     *      Returns card color
     * @return
     *      Returns an integer depending on Card Colour
     *      0 1 2 3
     *      H D S C
     */
    CardColour iGetCardColour() const;
    /**
     * @brief Compare
     *      Compares current card with an other card
     * @param other
     * @return
     *      Returns distance from the other card
     *      OtherCardValue - CurrentCardValue
     *          11         -        5           = 6
     */
    int Compare(const Card& other) const;

    /**
     * @brief CompareColours
     *      Comares two cards' colour
     * @param other
     *      Instance of Card
     * @return
     *      Returns true if two compared cards are same colour
     */
    bool CompareColours(const Card& other) const;

    /**
     * @brief CanBePlaced
     *      Determines whether card can be placed on OnThisCard
     * @param OnThisCard
     *      Card that is going to be parent of this Card
     * @return
     */
    bool CanBePlaced(const Card& OnThisCard) const;

};

#endif // CARD_H
