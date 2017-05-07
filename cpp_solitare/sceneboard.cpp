#include "sceneboard.h"

/// Base cards must be named like this:
/// one_H.png (Heath) one_D.png (Diamond) one_S.png (Spade) one_C.png (Club)
QString BaseCardPaths[14] =
{
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
    "placeholder" // same for all colours
};

QString charBaseCardColor[4] =
{
    "_H",
    "_D",
    "_S",
    "_C"
};
void SceneBoard::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug("DRAG ENTER SCENE");
}

SceneBoard::SceneBoard()
{
    float xx, yy, ww, hh;
    xx = 20;
    yy = 495;
    ww = 180;
    hh = 261;
    QGraphicsRectItem * rect = addRect(QRectF(0, 0, 1410, 1080));
    rect->setZValue(0);

    carditem *testPic = new carditem("A");
    testPic->setPixmap(QPixmap(":/Resources/ace_C.png").scaled(180, 261, Qt::KeepAspectRatio, Qt::FastTransformation));
    testPic->setZValue(1000);

    addItem(testPic);
    testPic->setPos(20, 20);

    carditem *testMove = new carditem("B");

    testMove->setPixmap(QPixmap(":/Resources/ace_C.png").scaled(180, 261, Qt::KeepAspectRatio, Qt::FastTransformation));
    testMove->setZValue(1001);
    testMove->setPos(20, 40);

    carditem *testMovero = new carditem("C");

    testMovero->setPixmap(QPixmap(":/Resources/ace_C.png").scaled(180, 261, Qt::KeepAspectRatio, Qt::FastTransformation));
    testMovero->setZValue(1001);
    testMovero->setPos(20, 60);

    addItem(testMove);
    addItem(testMovero);

    for (int i = 0; i < 7; i++)
    {
        carditem * item = new carditem("m");
        item->setPos(xx, yy);
        item->setPixmap(QPixmap(":/Resources/ace_C.png").scaled(180, 261, Qt::KeepAspectRatio, Qt::FastTransformation));
        addItem(item);
        xx = xx + ww + 18;
        yy = 495;
    }

    QGraphicsItem * item = itemAt(QPointF(50.0f, 50.0f), QTransform());
    // item == rect


}

void SceneBoard::StartGame()
{
    game = new cGameBoard();
    game->GenerateNewGame();
    LoadImages();
}

void SceneBoard::LoadImages()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            QImage(
        }
    }
}
