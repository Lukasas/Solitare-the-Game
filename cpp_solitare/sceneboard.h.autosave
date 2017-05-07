#ifndef SCENEBOARD_H
#define SCENEBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "viewboard.h"
#include "card.h"
#include "carditem.h"
#include "game_board.h"

class SceneBoard;

class CardPackClick : public QGraphicsPixmapItem
{
private:
    SceneBoard * board;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
public:
    CardPackClick(SceneBoard * board) { this->board = board; }
};

class PlaceForKing : public QGraphicsPixmapItem
{
private:
    int ID;
    SceneBoard * board;
    void dropEvent(QGraphicsSceneDragDropEvent * event) override;
public:
    PlaceForKing(SceneBoard * board, int ID);
    int GetID() const { return ID; }
};

class FinalPlace : public QGraphicsPixmapItem
{
private:
    int ID;
    SceneBoard * board;
    void dropEvent(QGraphicsSceneDragDropEvent * event) override;
public:
    FinalPlace(SceneBoard * board, int ID);
    int GetID() const { return ID; }

};

class SceneBoard : public QGraphicsScene
{
private:

    float xx, yy, ww, hh;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    cGameBoard * game;

    // 53 - Back | 0 - placeholder
    QPixmap *cards[54];

    carditem * packCard;

    CardPackClick * PackClick;

    QPixmap GetCardImgById(int ID);

    void LoadImages();
    void DrawLayout();
    void FirstDrawTheGame();
public:
    SceneBoard();
    void StartGame();
    void LoadGame(QString GameName);
    void PickNewCard();
    carditem * FindCardByName(QString name);
    void MoveCard(QString Which, carditem * Where);
    void MoveCard(QString Which, PlaceForKing * Where);
    void MoveCard(QString Which, FinalPlace * Where);
    // Create one for slots !
    //void MoveCard(carditem * Which, carditem * Where);

    void ShowCard(carditem * which);

};

#endif // SCENEBOARD_H
