#pragma once

#include <string>

#include <QThread>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressDialog>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QInputDialog>

using namespace std;

class UpdateThread : public QThread
{
    Q_OBJECT
private:
    QNetworkAccessManager *networkManager;
    QProgressDialog *progressDialog;
    QString ghPath;
    QString filePath;
    QString emuId;
    QEventLoop *loop;

    QString chosen_appimage;
public:
    explicit UpdateThread(QProgressDialog *progressDialog, QString ghPath, QString filePath, QString emuId, QObject *parent = nullptr) :
        QThread(parent),
        progressDialog(progressDialog),
        ghPath(ghPath),
        filePath(filePath),
        emuId(emuId)
    {}
    ~UpdateThread();

public slots:
    void receiveInput(const QString &choice);
signals:
    void requestInput(const QStringList &choices);
protected:
    void run() override;
signals:
    void resultReady();
};