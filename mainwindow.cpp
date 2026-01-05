#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <iostream>

GameDisplay::GameDisplay(Game *game, QWidget *parent) : QWidget(parent)
{
    QGridLayout *top_layout = new QGridLayout();
    QWidget *display = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    top_layout->addWidget(display, 0, 0);

    QLabel *icon = new QLabel();
    icon->setFixedSize(150, 150);
    icon->setPixmap(*(game->getIcon()));
    layout->addWidget(icon);

    QLabel *name_label = new QLabel();
    name_label->setText(game->getName().c_str());
    name_label->setAlignment(Qt::AlignHCenter);
    name_label->setStyleSheet("font: 20px;");
    name_label->setMaximumWidth(150);
    name_label->setWordWrap(true);
    layout->addWidget(name_label);

    QPushButton *button = new QPushButton();
    button->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    top_layout->addWidget(button, 0, 0);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    display->setLayout(layout);
    display->adjustSize();
    this->setLayout(top_layout);
    this->adjustSize();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    EMULATORS.fetchGames();
    EMULATORS.loadIcons();
    games = new QWidget(this);
    games->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *layout = new QGridLayout();

    for ( Game &game : *(EMULATORS.getGames()) )
    {
        game_displays.push_back(new GameDisplay(&game, this));
    }

    games->setLayout(layout);
    games->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
    for (GameDisplay *gd : game_displays)
    {
        delete gd;
    }
}

void MainWindow::rearrange()
{
    QGridLayout *games_layout = qobject_cast<QGridLayout*>(games->layout());
    int game_w = game_displays[0]->sizeHint().width() + 6;
    int per_row = floor(size().width() / game_w);
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
