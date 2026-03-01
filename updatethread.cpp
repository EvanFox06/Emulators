#include "updatethread.h"

void UpdateThread::run()
{
    loop = new QEventLoop();
    networkManager = new QNetworkAccessManager();
    progressDialog->setLabelText(tr("Backing up..."));
    QProcess process;
    process.setWorkingDirectory(filePath);
    process.start("zip", QStringList() << "-r" << ("../../backups/" + emuId + ".zip") << "games" << (emuId + ".config") << (emuId + ".home"));
    connect(&process, &QProcess::finished, loop, &QEventLoop::quit, Qt::SingleShotConnection);
    loop->exec();

    progressDialog->setLabelText(tr("Getting version info..."));
    QUrl url("https://api.github.com/repos/" + ghPath + "/releases/latest");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "EvanFox06");
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setRawHeader("X-GitHub-Api-Version", "2022-11-28");

    QNetworkReply *qReply = networkManager->get(request);
    
    connect(qReply, &QNetworkReply::finished, loop, &QEventLoop::quit);
    loop->exec();

    if (qReply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(qReply->readAll());
        QHash<QString, QString> dl_links;
        for (QJsonValue object : jsonResponse.object()["assets"].toArray())
        {
            QString name = object["name"].toString();
            if (name.toLower().endsWith(".appimage")) { dl_links[name] = object["browser_download_url"].toString(); }
        }
        QString dl_link;
        if (dl_links.size() == 1)
        {
            dl_link = dl_links[dl_links.keys()[0]];
        } else {
            emit requestInput(dl_links.keys());
            loop->exec();
            dl_link = dl_links[chosen_appimage];
        }
        QFile *file = new QFile(filePath + "/" + emuId);
        qDebug() << "Deleting file: " << file->fileName();
        file->remove();
        qDebug() << "Downloading: " << dl_link;
        progressDialog->setLabelText(tr("Downloading new version..."));
        file->open(QIODevice::WriteOnly);
        QNetworkReply *reply = networkManager->get(QNetworkRequest(QUrl(dl_link)));
        bool *set_range = new bool(false);
        connect(reply, &QNetworkReply::readyRead, this, [=]() {file->write(reply->readAll());});
        connect(reply, &QNetworkReply::downloadProgress, this, [=](qint64 bytesReceived, qint64 bytesTotal) {
            if (! *set_range) {
                progressDialog->setRange(0, bytesTotal);
                *set_range = true;
            }
            progressDialog->setValue(bytesReceived);
        });
        connect(reply, &QNetworkReply::finished, this, [=]() {
            file->flush();
            file->close();
            file->setPermissions(file->permissions() | QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);
            delete file;
            delete set_range;
            progressDialog->close();
            reply->deleteLater();
            emit resultReady();
            quit();
        });
    } else {
        qDebug() << "Network error: " << qReply->errorString();
    }
    qReply->deleteLater();
    exec();
}

void UpdateThread::receiveInput(const QString &choice)
{
    chosen_appimage = choice;
    loop->quit();
}

UpdateThread::~UpdateThread()
{
    delete loop;
    delete networkManager;
}