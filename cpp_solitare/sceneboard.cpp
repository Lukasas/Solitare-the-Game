#include "sceneboard.h"

#define CARD_OFFSET 40

#define BACK 53
#define PLACEHOLDER 0

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

QPixmap SceneBoard::GetCardImgById(int ID)
{
    return *(cards[ID]);
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



void SceneBoard::PickNewCard()
{
    Card * c = game->GetPackCard();
    if (c == 0)
    {
        if (game->PickPackCard())
        {
            c = game->GetPackCard();
            packCard = new carditem(*c, this);
            packCard->setPos(ww + 40, 20);
            packCard->setPixmap(GetCardImgById(c->GetID()));
            addItem(packCard);
        }
    }
    else
    {
        if (game->PickPackCard())
        {
            c = game->GetPackCard();
            packCard->ChangeCard(c);
            packCard->setPixmap(GetCardImgById(c->GetID()));
        }
    }
}

carditem *SceneBoard::FindCardByName(QString name)
{
    QList<QGraphicsItem*> all = items();
    for (int i = 0; i < all.count(); i++)
    {
        if (dynamic_cast<carditem*>(all.at(i)) != NULL)
        if (name.compare(((carditem*)all.at(i))->GetName()) == 0)
        {
            return ((carditem*)all.at(i));
        }
    }
}

void SceneBoard::MoveCard(QString Which, carditem *Where)
{
    carditem * movingCard = FindCardByName(Which);

    if (movingCard->Equal(*Where))
        return;

    if (Where->Equal(*packCard))
        return;

    CardPos wherePosition = game->GetCardLocation(Where);
    CardPos movingPosition = game->GetCardLocation(movingCard);
    if (movingPosition.ListID != -1) // Is in list
    {
        if (wherePosition.ListID != -1) // Is in list
        {
            std::vector<Card*> children = game->FindChildrenOfCard(movingPosition); // Needs to be here, becouse the position will change
            if (game->MoveCardToListFromList(movingPosition, wherePosition.ListID))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y() + CARD_OFFSET);
                movingCard->setZValue(Where->zValue() + 1 );
                float yy_help = movingCard->y() + CARD_OFFSET;
                for (int i = 0; i < children.size(); i++)
                {
                    carditem * kid = FindCardByName(((Card*)children.at(i))->GetName());
                    kid->setPos(movingCard->x(), yy_help);
                    kid->setZValue(movingCard->zValue() + (i + 1));
                    yy_help += CARD_OFFSET;
                }
            }
        }
        else // Have to be Slot
        {
            if (movingCard->iGetCardColour() == Where->iGetCardColour())
            {
                if (Where->Equal(*packCard))
                    return;

                if (game->MoveCardToSlotFromList(movingPosition, Where->iGetCardColour()))
                {
                    movingCard->setPos(Where->pos());
                    movingCard->setY(Where->y());
                    movingCard->setZValue(Where->zValue() + 1 );
                }
            }
        }

    }
    else
    {
        if (movingCard->Equal(*packCard)) // From pack
        {
            if (wherePosition.ListID != -1) // to List !
            {
                if (game->MoveCardToListFromPack(wherePosition.ListID))
                {
                    movingCard->setPos(Where->pos());
                    movingCard->setY(Where->y() + CARD_OFFSET);
                    movingCard->setZValue(Where->zValue() + 1 );
                    packCard = 0;
                    game->RemoveCardFromPack();
                }
            }
            else // From pack to Slot
            {
                if (movingCard->iGetCardColour() == Where->iGetCardColour())
                {
                    if (game->MoveCardToSlotFromPack(Where->iGetCardColour()))
                    {
                        movingCard->setPos(Where->pos());
                        movingCard->setY(Where->y());
                        movingCard->setZValue(Where->zValue() + 1 );
                    }
                }
            }

        }
        else // Have to be from Slot to List
        {
            if (game->MoveCardToListFromSlot(Where->iGetCardColour(), wherePosition.ListID))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y());
                movingCard->setZValue(Where->zValue() + 1 );
            }
        }
    }
}

void SceneBoard::MoveCard(QString Which, PlaceForKing *Where)
{
    carditem * movingCard = FindCardByName(Which);
    if (!movingCard->IsKing())
        return;
    CardPos movingPosition = game->GetCardLocation(movingCard);
    if (movingPosition.ListID != -1) // Is in list
    {
        std::vector<Card*> children = game->FindChildrenOfCard(movingPosition); // Needs to be here, becouse the position will change
        if (game->MoveCardToListFromList(movingPosition, Where->GetID()))
        {
            movingCard->setPos(Where->pos());
            movingCard->setY(Where->y());
            movingCard->setZValue(Where->zValue() + 1 );
            float yy_help = movingCard->y() + CARD_OFFSET;
            for (int i = 0; i < children.size(); i++)
            {
                carditem * kid = FindCardByName(((Card*)children.at(i))->GetName());
                kid->setPos(movingCard->x(), yy_help);
                kid->setZValue(movingCard->zValue() + (i + 1));
                yy_help += CARD_OFFSET;
            }
        }

    }
    else
    {
        if (movingCard->Equal(*packCard)) // From pack
        {
            if (game->MoveCardToListFromPack(Where->GetID()))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y());
                movingCard->setZValue(Where->zValue() + 1 );
                packCard = 0;
                game->RemoveCardFromPack();
            }
        }
        else // From slot
        {
            if (game->MoveCardToListFromSlot(movingCard->iGetCardColour(), Where->GetID()))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y());
                movingCard->setZValue(Where->zValue() + 1 );
            }
        }
    }
}

void SceneBoard::MoveCard(QString Which, FinalPlace *Where)
{
    carditem * movingCard = FindCardByName(Which);
    if (!movingCard->IsAce() && movingCard->iGetCardColour() != Where->GetID())
        return;
    CardPos movingPosition = game->GetCardLocation(movingCard);
    if (movingPosition.ListID != -1) // Is in list
    {
        if (game->MoveCardToSlotFromList(movingPosition, Where->GetID()))
        {
            movingCard->setPos(Where->pos());
            movingCard->setY(Where->y());
            movingCard->setZValue(Where->zValue() + 1 );
        }
    }
    else
    {
        if (movingCard->Equal(*packCard)) // From pack
        {
            if (game->MoveCardToSlotFromPack(Where->GetID()))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y());
                movingCard->setZValue(Where->zValue() + 1 );
                packCard = 0;
                game->RemoveCardFromPack();
            }
        }
    }
}

void SceneBoard::ShowCard(carditem *which)
{
    CardPos selected = game->GetCardLocation(which);
    if (game->CanBeShown(selected.ListID, selected.TopPos))
    {
        which->SetHidden(false);
        game->ShowCard(selected, true);
        which->setPixmap(GetCardImgById(which->GetID()));
    }
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

    PackClick = new CardPackClick(this);
    PackClick->setPixmap(*(cards[53]));
    PackClick->setPos(20, 20);


    QGraphicsPixmapItem *cardHolder = new QGraphicsPixmapItem();
    cardHolder->setPixmap(*(cards[0]));
    cardHolder->setPos(ww + 40, 20);

    addItem(PackClick);
    addItem(cardHolder);
    float xx_back = xx;
    for (int i = 0; i < 7; i++)
    {
        PlaceForKing * item = new PlaceForKing(this, i);
        item->setZValue(0);
        item->setPos(xx_back, yy);
        item->setPixmap(GetCardImgById(PLACEHOLDER));
        addItem(item);
        xx_back = xx_back + ww + 18;
        yy = 495;
    }

    xx_back = xx + 500;
    float yy_back = 20;
    for (int i = 0; i < 4; i++)
    {
        FinalPlace * item = new FinalPlace(this, i);
        item->setPos(xx_back, yy_back);
        item->setZValue(0);
        item->setPixmap(GetCardImgById(PLACEHOLDER));
        addItem(item);
        xx_back = xx_back + ww + 18;
    }
}

void SceneBoard::FirstDrawTheGame()
{
    float xx_back = xx;
    for (int i = 0; i < 7; i++)
    {
        std::vector<Card*> list = game->GetCardList(i);
        for (int cardID = 0; cardID < list.size(); cardID++)
        {
            carditem * item = new carditem(*list[cardID], this);
            CardPos loc = game->GetCardLocation(item);
            if (loc.ListID != i || loc.TopPos != cardID)
                throw;
            item->setPos(xx_back, yy);
            item->setZValue(cardID+1);
            yy += CARD_OFFSET;
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



void CardPackClick::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    board->PickNewCard();
}

void PlaceForKing::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QString name = event->mimeData()->text();
    SceneBoard *b = (SceneBoard*)board;
    b->MoveCard(name, this);
}

PlaceForKing::PlaceForKing(SceneBoard *board, int ID)
{
    this->board = board;
    this->ID = ID;
    setAcceptDrops(true);
}

void FinalPlace::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QString name = event->mimeData()->text();
    SceneBoard *b = (SceneBoard*)board;
    b->MoveCard(name, this);
}

FinalPlace::FinalPlace(SceneBoard *board, int ID)
{
    this->board = board;
    this->ID = ID;
    setAcceptDrops(true);
}
