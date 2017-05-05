#ifndef CARDQUEUE_H
#define CARDQUEUE_H

#include "card.h"

class CardList
{
private:
    struct ListItem
    {
        Card * card;
        ListItem * prev;
        ListItem * next;
    };
    ListItem * list;
    int count;
public:
    /**
     * @brief CardList
     * Creates an empty list
     */
    explicit CardList();

    ~CardList();

    void AddItem(Card * card);

    Card * RemoveItem();

    Card * GetCardAtIndex(int idx);
};

#endif // CARDQUEUE_H
