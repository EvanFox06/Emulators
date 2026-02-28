#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>

#include <iostream>

GameDisplay::GameDisplay(Game *game, QWidget *parent) : QWidget(parent), game(game)
{
    QGridLayout *top_layout = new QGridLayout();
    QWidget *display = new QWidget();
    QGridLayout *layout = new QGridLayout();
    top_layout->addWidget(display, 0, 0);

    QLabel *icon = new QLabel();
    icon->setFixedSize(150, 150);
    icon->setPixmap(*(game->getIcon()));
    layout->addWidget(icon, 0, 0);

    QLabel *emu_icon = new QLabel();
    emu_icon->setFixedSize(40, 40);
    emu_icon->setPixmap(*(game->getEmulator()->getIcon()));
    layout->addWidget(emu_icon, 0, 0);
    layout->setAlignment(emu_icon, Qt::AlignBottom | Qt::AlignRight);

    QLabel *name_label = new QLabel();
    name_label->setText(game->getName().c_str());
    name_label->setAlignment(Qt::AlignHCenter);
    name_label->setStyleSheet("font: 20px;");
    name_label->setMaximumWidth(150);
    name_label->setWordWrap(true);
    layout->addWidget(name_label, 1, 0);

    QPushButton *button = new QPushButton();
    button->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    top_layout->addWidget(button, 0, 0);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(button, &QPushButton::clicked, this, &GameDisplay::run);

    display->setLayout(layout);
    display->adjustSize();
    this->setLayout(top_layout);
    this->adjustSize();
}

void GameDisplay::run()
{
    game->run();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    EMULATORS.fetchGames();
    EMULATORS.loadIcons();

    dolphinAction = new QAction("Open Dolphin");
    connect(dolphinAction, &QAction::triggered, this, &MainWindow::openDolphin);
    menuBar()->addAction(dolphinAction);

    emulatorsAction = new QAction("Emulators");
    connect(emulatorsAction, &QAction::triggered, this, &MainWindow::openEmulatorsWindow);
    menuBar()->addAction(emulatorsAction);

    scroll = new QScrollArea();
    games = new QWidget();

    auto *layout = new QGridLayout();

    for ( Game *game : *(EMULATORS.getGames()) )
    {
        game_displays.push_back(new GameDisplay(game, games));
    }

    games->setLayout(layout);
    games->adjustSize();
    scroll->setWidget(games);
    auto *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(scroll);
    centralWidget()->setLayout(centralLayout);

    networkManager = new QNetworkAccessManager(this);
    emuVerReplies = EMULATORS.fetchLatestVersions(networkManager);
    for (EmulatorVersionReply *reply : emuVerReplies)
    {
        if (!(reply->shouldCheck()))
        {
            emulatorVers.push_back({reply->getEmulator()->getId(), "auto", "auto"});
            continue;
        }
        connect(reply->getReply(), &QNetworkReply::finished, this, [=]() { onFetchEmuVer(reply); });
    }
}

void MainWindow::onFetchEmuVer(EmulatorVersionReply *reply)
{
    QNetworkReply *qReply = reply->getReply();
    if (qReply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(qReply->readAll());
        emulatorVers.push_back({
            reply->getEmulator()->getId(),
            reply->getEmulator()->currentVersion(),
            jsonResponse.object()["tag_name"].toString().toStdString()
        });
    } else {
        qDebug() << "Network error: " << qReply->errorString();
    }
    qReply->deleteLater();
    if (emulatorVers.size() == emuVerReplies.size())
    {
        doneFetchingVers();
    }
}

void MainWindow::doneFetchingVers()
{
    emulatorsDialog = new EmulatorsDialog(&emulatorVers, networkManager, this);

    vector<array<string, 3>> outdated;
    for (array<string, 3> vers : emulatorVers) { if (vers[1] != vers[2]) { outdated.push_back(vers); } }

    string msg = "";
    for (int i = 0 ; i < outdated.size() ; i++)
    {
        array<string, 3> vers = outdated[i];
        msg += vers[0] + " is outdated!\ninstalled: " + vers[1] + "\nlatest: " + vers[2];
        if (i != (outdated.size() - 1)) { msg += "\n\n"; }
    }
    if (msg != "")
    {
        QMessageBox::warning
        ( 
            this, 
            tr("Emulator outdated!"), 
            tr(msg.c_str())
        );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dolphinAction;
    delete emulatorsAction;
    if (emulatorsDialog) {delete emulatorsDialog;}
    delete networkManager;
    for (GameDisplay *gd : game_displays)
    {
        delete gd;
    }
    for (EmulatorVersionReply *reply : emuVerReplies)
    {
        delete reply;
    }
}

void MainWindow::rearrange()
{
    QGridLayout *games_layout = qobject_cast<QGridLayout*>(games->layout());
    int game_w = game_displays[0]->sizeHint().width() + 6;
    int per_row = floor((scroll->viewport()->width() - 5) / game_w);
    if (!per_row) { per_row = 1; }
    for (GameDisplay *gd : game_displays)
    {
        games_layout->takeAt(games_layout->indexOf(gd));
    }
    for (int i = 0 ; i < game_displays.size() ; i++)
    {
        GameDisplay *gd = game_displays[i];
        games_layout->addWidget(gd, floor(i / per_row), i % per_row);
    }
    games->adjustSize();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    rearrange();
    QMainWindow::resizeEvent(event);
}
