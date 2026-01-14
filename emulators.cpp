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

Emulators::~Emulators()
{
    for (Game *game : games)
    {
        delete game;
    }
    delete dolphinProcess;
}

Emulators EMULATORS = Emulators();