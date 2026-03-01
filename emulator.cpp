#include "emulator.h"

Emulator::Emulator(string id, string ext, string gh_path) : id(id), ext(ext), gh_path(gh_path)
{
    path = "/home/evanfox/Emulators/emulators/" + id;
    run_path = path + "/" + id;
}

Emulator::~Emulator()
{
    delete icon;
    delete update_reply;
}

void Emulator::runGame(string game, QProcess *process)
{
    process->start(getRunPath().c_str(), QStringList() << game.c_str());
}

void Emulator::backup_update(QWidget *parent, QProcess *process, QNetworkAccessManager *manager, function<void()> when_done)
{
    QProgressDialog *progressDialog = new QProgressDialog(parent);
    progressDialog->setAutoClose(false);
    progressDialog->setAutoReset(false);
    progressDialog->setCancelButton(nullptr);
    progressDialog->setWindowFlag(Qt::WindowCloseButtonHint, false);
    progressDialog->show();
    
    UpdateThread *updateThread = new UpdateThread(progressDialog, tr(getGhPath().c_str()), tr(getPath().c_str()), tr(getId().c_str()), parent);
    connect(updateThread, &UpdateThread::resultReady, this, when_done);
    connect(updateThread, &UpdateThread::finished, updateThread, &QObject::deleteLater);
    connect(this, &Emulator::appimageChosen, updateThread, &UpdateThread::receiveInput);
    connect(updateThread, &UpdateThread::requestInput, this, [=](const QStringList &choices) {
        QString dl_link = QInputDialog::getItem(parent,
            tr("Select appimage"),
            tr("appimage:"),
            choices,
            0,
            false
        );
        emit appimageChosen(dl_link);
    });
    updateThread->start();
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

EmulatorVersionReply *Emulator::fetchLatestVersion(QNetworkAccessManager *manager)
{
    QUrl url((string("https://api.github.com/repos/") + getGhPath() + string("/releases/latest")).c_str());
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "EvanFox06");
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setRawHeader("X-GitHub-Api-Version", "2022-11-28");

    QNetworkReply *reply = manager->get(request);
    return new EmulatorVersionReply(this, reply);
}