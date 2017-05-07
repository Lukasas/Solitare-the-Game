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

    float xx, yy, ww, hh;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    cGameBoard * game;

    // 53 - Back | 0 - placeholder
    QPixmap *cards[54];


    void LoadImages();
    void DrawLayout();
    void FirstDrawTheGame();
public:
    SceneBoard();
    void StartGame();
    void LoadGame(QString GameName);

};

#endif // SCENEBOARD_H
