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

void Emulator::backup(QProcess *process)
{
    process->setWorkingDirectory(getPath().c_str());
    process->start("zip", QStringList() << "-r" << ("../../backups/" + getId() + ".zip").c_str() << "games" << (getId() + ".config").c_str() << (getId() + ".home").c_str());
}

void Emulator::backup(QProcess *process, const QObject *receiver, const QMetaMethod &method)
{
    process->setWorkingDirectory(getPath().c_str());
    process->start("zip", QStringList() << "-r" << ("../../backups/" + getId() + ".zip").c_str() << "games" << (getId() + ".config").c_str() << (getId() + ".home").c_str());
    // connect(process, &QProcess::finished, receiver, method, Qt::SingleShotConnection);
}

void Emulator::backup_update(QProcess *process, QNetworkAccessManager *manager, function<void()> when_done)
{
    process->setWorkingDirectory(getPath().c_str());
    process->start("zip", QStringList() << "-r" << ("../../backups/" + getId() + ".zip").c_str() << "games" << (getId() + ".config").c_str() << (getId() + ".home").c_str());
    connect(process, &QProcess::finished, this, [=]() {this->update(manager, when_done);}, Qt::SingleShotConnection);
}

void Emulator::update(QNetworkAccessManager *manager, function<void()> when_done)
{
    if (update_reply) {delete update_reply;}
    update_reply = this->fetchLatestVersion(manager);
    connect(update_reply->getReply(), &QNetworkReply::finished, this, [=]() {this->update2(manager, when_done);});
}

void Emulator::update2(QNetworkAccessManager *manager, function<void()> when_done)
{
    QNetworkReply *qReply = update_reply->getReply();
    if (qReply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(qReply->readAll());
        QString dl_link;
        for (QJsonValue object : jsonResponse.object()["assets"].toArray())
        {
            QString name = object["name"].toString();
            if (name.toLower().endsWith(".appimage") && !name.toLower().contains("wayland")) {
                dl_link = object["browser_download_url"].toString();
                break;
            }
        }
        QFile *file = new QFile(getRunPath().c_str());
        qDebug() << "Deleting file: " << getRunPath();
        file->remove();
        qDebug() << "Downloading: " << dl_link;
        file->open(QIODevice::WriteOnly);
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(dl_link)));
        connect(reply, &QNetworkReply::readyRead, this, [=]() {file->write(reply->readAll());});
        connect(reply, &QNetworkReply::downloadProgress, this, [=](qint64 bytesReceived, qint64 bytesTotal) {
            qDebug() << bytesReceived << " / " << bytesTotal;
        });
        connect(reply, &QNetworkReply::finished, this, [=]() {
            file->flush();
            file->close();
            file->setPermissions(file->permissions() | QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);
            delete file;
            when_done();
            reply->deleteLater();
            qDebug() << "Done";
        });
    } else {
        qDebug() << "Network error: " << qReply->errorString();
    }
    qReply->deleteLater();
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