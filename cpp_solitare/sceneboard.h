#ifndef SCENEBOARD_H
#define SCENEBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "viewboard.h"
#include "card.h"
#include "carditem.h"
#include "game_board.h"
class SceneBoard : public QGraphicsScene
{
private:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    cGameBoard * game;

    // 52 - Back | 53 - placeholder
    QImage *cards[54];

    void LoadImages();
public:
    SceneBoard();
    void StartGame();
};

#endif // SCENEBOARD_H
