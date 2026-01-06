#include "emulators.h"


Emulator::Emulator(string id, string ext, string gh_path) : id(id), ext(ext), gh_path(gh_path)
{
    path = "/home/evanfox/Emulators/emulators/" + id;
    run_path = path + "/" + id;
}

Emulator::~Emulator()
{
    delete icon;
}

void Emulator::runGame(string game, QProcess *process)
{
    process->start(getRunPath().c_str(), QStringList() << game.c_str());
}

void Emulator::loadIcon()
{
    string icon_path = path + "/" + id + ".png";
    if (std::filesystem::exists(icon_path))
    {
        QPixmap *temp_icon = new QPixmap(QString::fromStdString(icon_path));
        QPixmap icon_obj = temp_icon->scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        icon = new QPixmap(icon_obj);
        delete temp_icon;
    } else {
        icon = new QPixmap(40, 40);
        icon->fill(Qt::white);
    }
}

void Emulator::fetchGameIDs()
{
    game_ids.clear();
    for (const auto &dir : filesystem::directory_iterator(getPath() + "/games"))
    {
        string filename = dir.path().filename().string();
        if (filename != ".gitignore")
        {
            game_ids.push_back(filename);
        }
    }
}

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

Emulators::~Emulators()
{
    for (Game *game : games)
    {
        delete game;
    }
    delete dolphinProcess;
}

Emulators EMULATORS = Emulators();