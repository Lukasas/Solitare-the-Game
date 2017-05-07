#ifndef CARD_H
#define CARD_H

#include <QString>

enum Cards
{
    PLACEHOLDER,    CACE,    C2,
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


class Card
{
private:
    Cards value;
    CardColour colour;
    bool hidden;
    QString name;
public:
    explicit Card(Cards CardId, CardColour colour, bool hidden, QString name);
    explicit Card(int ID);
    explicit Card(const Card & otherone);

    /**
     * @brief SetName
     * Sets an helping name
     * @param name
     * Name of card
     */
    void SetName(QString name);

    void SetCardValue(Cards value);

    void SetCardColour(CardColour colour);

    /**
     * @brief GetName
     * Returns a helping name of card
     * @return
     * Property name
     */
    QString GetName() const;

    /**
     * @brief iGetCardValue
     *      Returns card value (1>K)
     * @return
     *      Returns an integer depenting on selected Card
     *      0 1 2 3 4 5 6 7 8 9 10 11 12 13
     *      P A 2 3 4 5 6 7 8 9 10  J  Q  K
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
     * @brief Card::Equal
     * Compare if the "other" card is exactly same as the asking one
     * @param other
     * Just a Card
     * @return
     * True if colour and value are same
     */
    bool Equal(const Card & other) const;

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

    bool IsHidden() const;

    void SetHidden(bool h);

    int GetID();

    bool IsKing() const;

    bool IsAce() const;
};

#endif // CARD_H
