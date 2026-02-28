#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <QPixmap>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMetaMethod>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

using namespace std;

class EmulatorVersionReply;

class Emulator : QObject
{
    Q_OBJECT

private:
    string id;
    string ext;
    string gh_path;
    string path;
    string run_path;
    QPixmap *icon;
    vector<string> game_ids;
    EmulatorVersionReply *update_reply;
    friend class Emulators;

    Emulator(string id, string ext, string gh_path);
    void update2(QNetworkAccessManager *manager, function<void()> when_done);
    void update2(QNetworkAccessManager *manager, const QObject *receiver, const QMetaMethod &method);

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
    virtual string currentVersion() { return ""; }
    virtual EmulatorVersionReply *fetchLatestVersion(QNetworkAccessManager *manager);

    void fetchGameIDs();
    void loadIcon();

    void backup(QProcess *process);
    void backup(QProcess *process, const QObject *receiver, const QMetaMethod &method);
    void backup_update(QProcess *process, QNetworkAccessManager *manager, function<void()> when_done);
    void update(QNetworkAccessManager *manager, function<void()> when_done);
    void update(QNetworkAccessManager *manager, const QObject *receiver, const QMetaMethod &method);
};

class EmulatorVersionReply
{
    QNetworkReply *reply;
    Emulator *emulator;
    bool check;

public:
    EmulatorVersionReply(Emulator *emulator): emulator(emulator), check(false) {};
    EmulatorVersionReply(Emulator *emulator, QNetworkReply *reply): emulator(emulator), reply(reply), check(true) {};
    QNetworkReply *getReply() { return reply; }
    Emulator *getEmulator() { return emulator; }
    bool shouldCheck() { return check; }
};