/**
  File: carditem.h
  Author: Lukáš Chábek (xchabe00)
  Description: Header file of carditem.cpp
  **/

#ifndef CARDITEM_H
#define CARDITEM_H

#include <QGraphicsSceneEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "card.h"
#include "game_board.h"
#include <QMimeData>
#include <QDrag>


class carditem : public QGraphicsPixmapItem, public Card
{
private:
    int CDlistID;
    int cardPos;

    void * Scene;

    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent * event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    cGameBoard * board;
public:
    carditem(Card & C, void * b);

    void ChangeCard(Card * C);

    void SetCardPosition(int _CDlistID, int zIndex, int _cardPos);

};

#endif // CARDITEM_H
