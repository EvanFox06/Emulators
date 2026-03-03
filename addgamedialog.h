#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QLineEdit>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "emulators.h"

class AddGameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddGameDialog(QWidget *parent = nullptr);
private:
    QGridLayout *layout;
    QLabel *nameLabel;
    QLineEdit *nameInput;
    QLabel *imageUrlLabel;
    QLineEdit *imageUrlInput;
    QPushButton *imageFetchButton;
    QLabel *chooseFileLabel;
    QPushButton *chooseFileButton;
    QLabel *imagePreview;
    QPushButton *addGameButton;

    QString fileName;
    QPixmap image;
    QNetworkAccessManager networkManager;
private slots:
    void openFileDialog();
    void fetchImage();
    void addGame();
};