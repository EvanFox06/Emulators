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

void Emulator::loadIcon()
{
    string icon_path = path + "/icon.png";
    if (std::filesystem::exists(icon_path))
    {
        icon = new QPixmap(QString::fromStdString(icon_path));
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

Game::Game(string name, Emulator *emulator) : name(name), emulator(*emulator)
{
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

void Emulators::fetchGames()
{
    games.clear();
    for (Emulator *emulator : ALL)
    {
        emulator->fetchGameIDs();
        for (string id : *(emulator->getGames()))
        {
            games.push_back(Game(id, emulator));
        }
    }
    sort(games.begin(), games.end());
}

void Emulators::loadIcons()
{
    for (Emulator *emulator : ALL)
    {
        emulator->loadIcon();
    }
}

Emulators EMULATORS = Emulators();