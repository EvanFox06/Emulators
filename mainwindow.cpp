#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <iostream>
#include <filesystem>

Emulator::Emulator(string id, string ext, string gh_path) : id(id), ext(ext), gh_path(gh_path)
{
    this->path = "/home/evanfox/Emulators/emulators/" + id;
    string icon_path = this->path + "/icon.png";
    if (std::filesystem::exists(icon_path))
    {
        this->icon = &QPixmap(QString::fromStdString(icon_path));
    } else {
        this->icon = &QPixmap(40, 40);
        this->icon->fill(Qt::white);
    }
}

Game::Game(string name, Emulator *emulator) : name(name), emulator(*emulator)
{
    this->path = emulator->getPath() + "/" + name;
    string icon_path = this->path + "/icon.png";
    if (std::filesystem::exists(icon_path))
    {
        this->icon = &QPixmap(QString::fromStdString(icon_path));
    } else {
        this->icon = &QPixmap(40, 40);
        this->icon->fill(Qt::white);
    }
}

GameDisplay::GameDisplay(QWidget *parent) : QWidget(parent)
{
    QPixmap image(150, 150);
    image.fill(Qt::white);

    QGridLayout *top_layout = new QGridLayout();
    QWidget *display = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    top_layout->addWidget(display, 0, 0);

    QLabel *icon = new QLabel();
    icon->setFixedSize(150, 150);
    icon->setPixmap(image);
    layout->addWidget(icon);

    QLabel *name_label = new QLabel();
    name_label->setText("Game Name");
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
    auto *button = new GameDisplay(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
