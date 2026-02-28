#pragma once

#include "emulators.h"

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

class EmuOptions : public QWidget
{
    Q_OBJECT

public:
    explicit EmuOptions(Emulator *emulator, array<string, 3> ver, QProcess *process, QNetworkAccessManager *manager, QWidget *parent = nullptr);
    ~EmuOptions();
private:
    Emulator *emulator;
    QGridLayout *layout;
    QLabel *nameLabel;
    QLabel *verLabel;
    QPushButton *updateButton;
};

class EmulatorsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmulatorsDialog(vector<array<string, 3>> *vers, QNetworkAccessManager *manager, QWidget *parent = nullptr);
    ~EmulatorsDialog();

private:
    QVBoxLayout *mainLayout;
    vector<EmuOptions*> emulators;
    QProcess *process;
};
