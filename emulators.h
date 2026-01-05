#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <QPixmap>

using namespace std;

class Emulator
{
private:
    string id;
    string ext;
    string gh_path;
    string path;
    string run_path;
    QPixmap *icon;
    vector<string> game_ids;
    friend class Emulators;

    Emulator(string id, string ext, string gh_path);

public:
    ~Emulator();
    string getId() { return id; }
    string getExt() { return ext; }
    string getGhPath() { return gh_path; }
    string getPath() { return path; }
    string getRunPath() { return run_path; }
    QPixmap *getIcon() { return icon; }
    vector<string> *getGames() { return &game_ids; }

    virtual string getRunCmd(string game) { return ""; }
    virtual string installedVersion() { return ""; }

    void fetchGameIDs();
    void loadIcon();
    // TODO function to check latest version
};

class Game
{
    string name;
    string path;
    string run_path;
    Emulator emulator;
    QPixmap icon;

public:
    Game(string name, Emulator *emulator);
    string getName() { return name; }
    string getPath() { return path; }
    string getRunPath() { return run_path; }
    Emulator *getEmulator() { return &emulator; }
    QPixmap *getIcon() { return &icon; }

    bool operator < (const Game &game) const { return (this->name < game.name); }
};

class Emulators
{
    vector<Game> games;
    // TODO functions to check installed versions for all emulators
    class _MelonDS : public Emulator
    {
    public:
        _MelonDS() : Emulator("melonds", "nds", "melonDS-emu/melonDS") {}
        string getRunCmd(string game) override { return getRunPath() + " \"" + game + "\""; }
    };

    class _Mgba : public Emulator
    {
    public:
        _Mgba() : Emulator("mgba", "gba", "mgba-emu/mgba") {}
        string getRunCmd(string game) override { return getRunPath() + " \"" + game + "\""; }
    };

public:
    _MelonDS MELONDS = _MelonDS();
    _Mgba MGBA = _Mgba();
    array<Emulator*, 2> ALL = {&MELONDS, &MGBA};
    void fetchGames();
    void loadIcons();

    vector<Game> *getGames() { return &games; }

};

extern Emulators EMULATORS;