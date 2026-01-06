#pragma once

#include "emulators.h"

#include <QMainWindow>
#include <QResizeEvent>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class GameDisplay : public QWidget
{
    Q_OBJECT


public:
    Game *game;
    GameDisplay(Game *game, QWidget *parent = nullptr);
public slots:
    void run();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    vector<GameDisplay*> game_displays;
    QWidget *games;
    QAction *dolphinAction;

public:
    
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void rearrange();
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void openDolphin() { EMULATORS.startDolphin(); };
};
