#pragma once

#include <iostream>
#include <string>
#include <QMainWindow>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

class Emulator
{
private:
    string id;
    string ext;
    string gh_path;
    string path;
    string run_path;
    QPixmap *icon;
    friend class Emulators;

    explicit Emulator(string id, string ext, string gh_path);

public:
    string getId() { return id; }
    string getExt() { return ext; }
    string getGhPath() { return gh_path; }
    string getPath() { return path; }
    string getRunPath() { return run_path; }
    QPixmap *getIcon() { return icon; }
    string getRunCmd(string game) { return ""; }
    string installedVersion() { return ""; }
    // TODO function to check latest version
};

class Game
{
    string name;
    string path;
    string run_path;
    Emulator emulator;
    QPixmap *icon;

public:
    explicit Game(string name, Emulator *emulator);
    string getName() { return name; }
    string getPath() { return path; }
    string getRunPath() { return run_path; }
    Emulator getEmulator() { return emulator; }
    QPixmap *getIcon() { return icon; }
};

class Emulators
{
    // TODO functions to check installed versions for all emulators
    class _MelonDS : public Emulator
    {
    public:
        _MelonDS() : Emulator("melonds", "nds", "melonDS-emu/melonDS") {}
        string getRunCmd(Game *game) { return getRunPath() + " \"" + game->getPath() + "\""; }
    };

public:
    Emulator *MELONDS = new _MelonDS();
    const array<Emulator*, 1> ALL = {MELONDS};
};

class GameDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit GameDisplay(QWidget *parent = nullptr);
};
