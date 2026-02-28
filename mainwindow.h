#pragma once

#include "emulatorsdialog.h"

#include <QMainWindow>
#include <QResizeEvent>
#include <QGridLayout>
#include <QScrollArea>
#include <QNetworkReply>

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
    QScrollArea *scroll;
    QAction *dolphinAction;
    QAction *emulatorsAction;
    EmulatorsDialog *emulatorsDialog = nullptr;
    QNetworkAccessManager *networkManager;
    vector<EmulatorVersionReply*> emuVerReplies;
    vector<array<string, 3>> emulatorVers;

    void onFetchEmuVer(EmulatorVersionReply *reply);
    void doneFetchingVers();

public:
    
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void rearrange();
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void openDolphin() { EMULATORS.startDolphin(); };
    void openEmulatorsWindow() { if (emulatorsDialog) {emulatorsDialog->show();} };
};
