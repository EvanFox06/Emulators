#include "emulators.h"


Emulator::Emulator(string id, string ext, string gh_path) : id(id), ext(ext), gh_path(gh_path)
{
    path = "/home/evanfox/Emulators/emulators/" + id;
    run_path = path + "/" + id;
    string icon_path = path + "/icon.png";
    if (std::filesystem::exists(icon_path))
    {
        icon = QPixmap(QString::fromStdString(icon_path));
    } else {
        icon = QPixmap(40, 40);
        icon.fill(Qt::white);
    }
}

void Emulator::fetchGameIDs()
{
    game_ids.clear();
    for (const auto &dir : filesystem::directory_iterator(getPath()))
    {
        game_ids.push_back(dir.path().filename().string());
    }
}

Game::Game(string name, Emulator *emulator) : name(name), emulator(*emulator)
{
    path = emulator->getPath() + "/games/" + name;
    run_path = path + "/game." + emulator->getExt();
    string icon_path = path + "/icon.png";
    if (std::filesystem::exists(icon_path))
    {
        icon = QPixmap(QString::fromStdString(icon_path));
    } else {
        icon = QPixmap(40, 40);
        icon.fill(Qt::white);
    }
}