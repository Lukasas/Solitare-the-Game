#include "mainwindow.h"
#include "card.h"
#include "carditem.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    countGames = 0;
    grid = new QGridLayout;

    QWidget * widget = new QWidget;
    setCentralWidget(widget);
    widget->setLayout(grid);

    resize(600, 400);
    createActions();
    createMenus();
    setWindowTitle("Solitare !");

    QMessageBox::information(this, "Vítejte!", "Vyberte požadovaný počet her pro start !", QMessageBox::Ok);

}



MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *)
{
    ResizeGames();
}


void MainWindow::AddGame()
{
    view[countGames] = new QGraphicsView(&(scene[countGames]));
    scene[countGames].StartGame();
    grid->addWidget(view[countGames], floor(countGames / 2), countGames % 2);
    games[countGames]->menuAction()->setVisible(true);
    countGames++;
}

void MainWindow::RemoveGame()
{
    //scene[countGames].SaveCheck
    //scene[countGames].EndGame -> gameboard.clear

    countGames--;
    games[countGames]->menuAction()->setVisible(false);
    scene[countGames].clear();
    scene[countGames].update();
    grid->removeWidget(view[countGames]);
    delete view[countGames];
}


void MainWindow::createActions()
{
     QSignalMapper *sigmapCountGame = new QSignalMapper(this);
     connect (sigmapCountGame, SIGNAL(mapped(int)), this, SLOT(ChangeCountGames(int)));

     QSignalMapper *signalMapperNew = new QSignalMapper(this);
     connect(signalMapperNew, SIGNAL(mapped(int)), this, SLOT(NewGame(int)));
     QSignalMapper *signalMapperSave = new QSignalMapper(this);
     connect(signalMapperSave, SIGNAL(mapped(int)), this, SLOT(SaveGame(int)));
     QSignalMapper *signalMapperLoad = new QSignalMapper(this);
     connect(signalMapperLoad, SIGNAL(mapped(int)), this, SLOT(LoadGame(int)));
     QSignalMapper *signalMapperBack = new QSignalMapper(this);
     connect(signalMapperBack, SIGNAL(mapped(int)), this, SLOT(BackGame(int)));


     for (int i = 0; i < 4; i++)
     {
        NewGameAc[i] = new QAction("Nová hra", this);
        signalMapperNew->setMapping(NewGameAc[i], i);
        connect(NewGameAc[i], SIGNAL(triggered(bool)), signalMapperNew, SLOT(map()));

        SaveGameAc[i] = new QAction("Uložit hru", this);
        signalMapperSave->setMapping(SaveGameAc[i], i);
        connect(SaveGameAc[i], SIGNAL(triggered(bool)), signalMapperSave, SLOT(map()));

        LoadGameAc[i] = new QAction("Nahrát hru", this);
        signalMapperLoad->setMapping(LoadGameAc[i], i);
        connect(LoadGameAc[i], SIGNAL(triggered(bool)), signalMapperLoad, SLOT(map()));

        BackGameAc[i] = new QAction("Krok zpět", this);
        signalMapperBack->setMapping(BackGameAc[i], i);
        connect(BackGameAc[i], SIGNAL(triggered(bool)), signalMapperBack, SLOT(map()));

        GameSelect[i] = new QAction(QString::number(i+1), this);
        sigmapCountGame->setMapping(GameSelect[i], i+1);
        connect(GameSelect[i], SIGNAL(triggered(bool)), sigmapCountGame, SLOT(map()));

     }

}

void MainWindow::createMenus()
{
    gameMenu = menuBar()->addMenu(tr("&Hra"));
    pickNum = gameMenu->addMenu(tr("&Počet Her"));

    for (int i = 0; i < 4; i++)
    {
        pickNum->addAction(GameSelect[i]);
        games[i] = menuBar()->addMenu(QString("Game ") + QString::number(i+1));
        games[i]->addAction(NewGameAc[i]);
        games[i]->addAction(SaveGameAc[i]);
        games[i]->addAction(LoadGameAc[i]);
        games[i]->addAction(BackGameAc[i]);
        games[i]->menuAction()->setVisible(false);
    }

}


void MainWindow::ChangeCountGames(int Num)
{
    int countBack = countGames;
    if (Num > countGames)
    {
        for (int i = 0; i < (Num - countBack); i++)
        {
            AddGame();
        }
    }
    else if (Num < countGames)
    {
        for (int i = 0; i < (countBack - Num); i++)
        {
            RemoveGame();
        }
    }

    grid->update();
    RefreshViewport();
    ResizeGames();
}

void MainWindow::ResizeGames()
{
    for (int i = 0; i < countGames; i++)
    {
        scene[i].update();
        view[i]->viewport()->update();
        view[i]->fitInView(scene[i].sceneRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::RefreshViewport()
{
    for (int i = 0; i < countGames; i++)
    {
        scene[i].update();
        view[i]->viewport()->update();
        view[i]->fitInView(scene[i].sceneRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::SaveGame(int gameID)
{
    if (QMessageBox::information(this, "Save Game", "Chcete uložit hru číslo " + QString::number(gameID + 1),
                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        return;


    QString saveName = QFileDialog::getSaveFileName(this, "Save Game", "", tr("Game File (*.game)"));
    if (saveName == "")
        return;

    if (!scene[gameID].SaveGame((saveName.toStdString())))
        QMessageBox::warning(this, "Save Game", "Game was NOT saved successfully", QMessageBox::Ok);
    else
        QMessageBox::information(this, "Save Game", "Game was saved successfully", QMessageBox::Ok);
}

void MainWindow::LoadGame(int gameID)
{
    QString loadName = QFileDialog::getOpenFileName(this, "Load Game", "", "Game File (*.game)");
    if (loadName == "")
        return;
    if (!scene[gameID].LoadGame((loadName.toStdString())))
        QMessageBox::warning(this, "Load Game", "Game was NOT loaded successfully", QMessageBox::Ok);
    else
        QMessageBox::information(this, "Load Game", "Game was loaded successfully", QMessageBox::Ok);
}

void MainWindow::BackGame(int gameID)
{
    scene[gameID].StepBack();
}

void MainWindow::NewGame(int gameID)
{
    scene[gameID].StartGame();
}
