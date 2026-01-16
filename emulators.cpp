#include "emulators.h"

Game::Game(string name, Emulator *emulator) : name(name), emulator(emulator)
{
    process = new QProcess();
    path = emulator->getPath() + "/games/" + name;
    run_path = path + "/game." + emulator->getExt();
    string icon_path = path + "/icon.png";
    if (std::filesystem::exists(icon_path))
    {
        icon = QPixmap(QString::fromStdString(icon_path)).scaled(150, 150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    } else {
        icon = QPixmap(150, 150);
        icon.fill(Qt::white);
    }
}

Game::~Game()
{
    delete process;
}

void Game::run()
{
    getEmulator()->runGame(getRunPath(), process);
}

void Emulators::fetchGames()
{
    games.clear();
    for (Emulator *emulator : ALL)
    {
        emulator->fetchGameIDs();
        for (string id : *(emulator->getGames()))
        {
            games.push_back(new Game(id, emulator));
        }
    }
    sort(games.begin(), games.end(), Game::comparePtr);
}

void Emulators::loadIcons()
{
    for (Emulator *emulator : ALL)
    {
        emulator->loadIcon();
    }
}

void Emulators::startDolphin()
{
    dolphinProcess->start(DOLPHIN.getRunPath().c_str());
}

string Emulators::_MelonDS::currentVersion()
{
    QProcess getVersion;
    getVersion.start(getRunPath().c_str(), QStringList() << "--help");
    getVersion.waitForFinished();
    auto out = string(getVersion.readAllStandardOutput());
    int start = out.find(' ') + 1;
    return out.substr(start, out.find('\n') - start);
}

string Emulators::_Mgba::currentVersion()
{
    QProcess getVersion;
    getVersion.start(getRunPath().c_str(), QStringList() << "--version");
    getVersion.waitForFinished();
    auto out = string(getVersion.readAllStandardOutput());
    int start = out.find(' ') + 1;
    return out.substr(start, out.find(' ', start) - start);
}

string Emulators::_Azahar::currentVersion()
{
    QProcess getVersion;
    getVersion.start(getRunPath().c_str(), QStringList() << "-v");
    getVersion.waitForFinished();
    auto out = string(getVersion.readAllStandardOutput());
    int start = out.find(' ') + 1;
    return out.substr(start, out.length() - start - 1);
}

vector<EmulatorVersionReply*> Emulators::fetchLatestVersions(QNetworkAccessManager *manager)
{
    vector<EmulatorVersionReply*> replies;
    for (Emulator *emulator : ALL)
    {
        replies.push_back(emulator->fetchLatestVersion(manager));
    }
    return replies;
}

Emulators::~Emulators()
{
    for (Game *game : games)
    {
        delete game;
    }
    delete dolphinProcess;
}

Emulators EMULATORS = Emulators();