#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <QPixmap>
#include <QProcess>

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

    void runGame(string game, QProcess *process);
    virtual string installedVersion() { return ""; }

    void fetchGameIDs();
    void loadIcon();
    // TODO function to check latest version
};