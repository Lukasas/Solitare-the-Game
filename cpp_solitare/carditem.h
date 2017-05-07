#ifndef CARDITEM_H
#define CARDITEM_H

#include <QGraphicsSceneEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "card.h"

#include <QMimeData>
#include <QDrag>
class carditem : public QGraphicsPixmapItem, public Card
{
private:
    int zIndex;
    int CDlistID;
    int cardPos;

    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent * event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
public:
    char * name;
    carditem(char * name);

    void SetCardPosition(int _CDlistID, int zIndex, int _cardPos);

};

#endif // CARDITEM_H
