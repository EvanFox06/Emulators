#pragma once

#include "emulator.h"

#include <QDebug>
#include <QStringList>
#include <QAction>

class Game
{
    string name;
    string path;
    string run_path;
    Emulator *emulator;
    QPixmap icon;
    QProcess *process;

public:
    Game(string name, Emulator *emulator);
    ~Game();
    string getName() { return name; }
    string getPath() { return path; }
    string getRunPath() { return run_path; }
    Emulator *getEmulator() { return emulator; }
    QPixmap *getIcon() { return &icon; }

    bool operator < (const Game &game) const { return (this->name < game.name); }
    static bool comparePtr(Game *a, Game *b) {return *a < *b;}

    void run();
};

class Emulators
{
    vector<Game*> games;

    class _MelonDS : public Emulator
    {
    public:
        _MelonDS() : Emulator("melonds", "nds", "melonDS-emu/melonDS") {}
        string currentVersion();
    };

    class _Mgba : public Emulator
    {
    public:
        _Mgba() : Emulator("mgba", "gba", "mgba-emu/mgba") {}
        string currentVersion();
    };

    class _Dolphin : public Emulator
    {
    public:
        _Dolphin() : Emulator("dolphin", "rvz", "dolphin-emu/dolphin") {}
        string currentVersion() {return "auto";}
        EmulatorVersionReply *fetchLatestVersion(QNetworkAccessManager *manager) { return new EmulatorVersionReply(this); }
    };

    class _Azahar : public Emulator
    {
    public:
        _Azahar() : Emulator("azahar", "cci", "azahar-emu/azahar") {}
        string currentVersion();
    };

public:
    ~Emulators();
    _MelonDS MELONDS = _MelonDS();
    _Mgba MGBA = _Mgba();
    _Dolphin DOLPHIN = _Dolphin();
    _Azahar AZAHAR = _Azahar();
    array<Emulator*, 4> ALL = {&MELONDS, &MGBA, &DOLPHIN, &AZAHAR};

    QProcess *dolphinProcess = new QProcess();
    void startDolphin();

    void fetchGames();
    void loadIcons();
    vector<EmulatorVersionReply*> fetchLatestVersions(QNetworkAccessManager *manager);

    vector<Game*> *getGames() { return &games; }

};

extern Emulators EMULATORS;