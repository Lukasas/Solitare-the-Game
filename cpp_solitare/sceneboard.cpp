#include "sceneboard.h"

/// Base cards must be named like this:
/// one_H.png (Heath) one_D.png (Diamond) one_S.png (Spade) one_C.png (Club)
QString BaseCardPaths[14] =
{
    "ace", // same for all colours
    "two", // same for all colours
    "three", // same for all colours
    "four", // same for all colours
    "five", // same for all colours
    "six", // same for all colours
    "seven", // same for all colours
    "eight", // same for all colours
    "nine", // same for all colours
    "ten", // same for all colours
    "jack", // same for all colours
    "queen", // same for all colours
    "king", // same for all colours
    "placeholder", // same for all colours
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
    xx = 20;
    yy = 495;
    ww = 180;
    hh = 261;
    LoadImages();
    DrawLayout();
}

void SceneBoard::StartGame()
{
    game = new cGameBoard();
    game->GenerateNewGame();
    FirstDrawTheGame();
}

void SceneBoard::LoadGame(QString GameName)
{
    game = new cGameBoard();
    FirstDrawTheGame();
}

void SceneBoard::LoadImages()
{
    int currentID = 1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            QString test = ":/Resources/" + BaseCardPaths[j] + charBaseCardColor[i] + ".png";
            cards[currentID] = new QPixmap(QPixmap(test).scaled(180, 261, Qt::KeepAspectRatio, Qt::FastTransformation));
            if (cards[currentID]->width() == 0)
            {
                qDebug("Error!");
            }
            currentID++;
        }
    }
    cards[0] = new QPixmap(QPixmap(":/Resources/placeholder.png").scaled(180, 261, Qt::KeepAspectRatio, Qt::FastTransformation));
    cards[53] = new QPixmap(QPixmap(":/Resources/back.png").scaled(180, 261, Qt::KeepAspectRatio, Qt::FastTransformation));
}

// THIS NEEDS TO BE REPLACED WITH NEW QGraphicsItem !!!
#define PLAYCEHOLDERTYPE carditem
void SceneBoard::DrawLayout()
{
    // RESOLUTION <= Control + F

    // Magic numbers for Scene muhuhahaha

    QGraphicsRectItem * rect = addRect(QRectF(0, 0, 1410, 1080));
    rect->setZValue(0);

    PLAYCEHOLDERTYPE *testPic = new carditem("A");
    testPic->setPixmap(*(cards[0]));
    testPic->setPos(20, 20);


    PLAYCEHOLDERTYPE *testPics = new carditem("A");
    testPics->setPixmap(*(cards[0]));
    testPics->setPos(ww + 40, 20);

    addItem(testPic);
    addItem(testPics);
    float xx_back = xx;
    for (int i = 0; i < 7; i++)
    {
        PLAYCEHOLDERTYPE * item = new carditem("m");
        item->setPos(xx_back, yy);

        xx_back = xx_back + ww + 18;
        yy = 495;
    }

    for (int i = 0; i < 4; i++)
    {
        PLAYCEHOLDERTYPE * item = new carditem("m");
        item->setPos(xx_back, yy);

        xx_back = xx_back + ww + 18;
        yy = 495;
    }
}



void SceneBoard::FirstDrawTheGame()
{

    carditem *testPic = new carditem("A");
    testPic->setPixmap(*(cards[53]));
    testPic->setPos(20, 20);

    addItem(testPic);
    float xx_back = xx;
    for (int i = 0; i < 7; i++)
    {
        std::vector<Card*> list = game->GetCardList(i);
        for (int cardID = 0; cardID < list.size(); cardID++)
        {
            carditem * item = new carditem("m");
            item->setPos(xx_back, yy);
            yy += 40;
            if (!list[cardID]->IsHidden())
                item->setPixmap(*(cards[list[cardID]->GetID()]));
            else
                item->setPixmap(*(cards[53]));
            addItem(item);
        }


        xx_back = xx_back + ww + 18;
        yy = 495;
    }
}


