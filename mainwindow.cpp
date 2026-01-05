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
    QWidget *container = new QWidget(this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *layout = new QHBoxLayout();

    for ( Game &game : *(EMULATORS.getGames()) )
    {
        auto *gd = new GameDisplay(&game, this);
        game_displays.push_back(gd);
        layout->addWidget(gd);
    }

    container->setLayout(layout);
    container->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
    for (GameDisplay *gd : game_displays)
    {
        delete gd;
    }
}
