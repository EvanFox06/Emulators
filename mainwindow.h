#pragma once

#include "emulators.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class GameDisplay : public QWidget
{
    Q_OBJECT

public:
    GameDisplay(Game *game, QWidget *parent = nullptr);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    vector<GameDisplay*> game_displays;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
