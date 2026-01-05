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
    QPixmap icon;
    vector<string> game_ids;
    friend class Emulators;

    Emulator(string id, string ext, string gh_path);

public:
    string getId() { return id; }
    string getExt() { return ext; }
    string getGhPath() { return gh_path; }
    string getPath() { return path; }
    string getRunPath() { return run_path; }
    QPixmap *getIcon() { return &icon; }
    vector<string> *getGames() { return &game_ids; }

    string getRunCmd(string game) { return ""; }
    string installedVersion() { return ""; }

    void fetchGameIDs();
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
    Emulator getEmulator() { return emulator; }
    QPixmap *getIcon() { return &icon; }
};

class Emulators
{
    vector<Game> games;
    // TODO functions to check installed versions for all emulators
    class _MelonDS : public Emulator
    {
    public:
        _MelonDS() : Emulator("melonds", "nds", "melonDS-emu/melonDS") {}
        string getRunCmd(Game *game) { return getRunPath() + " \"" + game->getPath() + "\""; }
    };

public:
    _MelonDS MELONDS = _MelonDS();
    array<Emulator*, 1> ALL() { return {&MELONDS}; }
    void fetchGames();

};