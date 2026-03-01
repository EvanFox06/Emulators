#include "emulatorsdialog.h"

EmulatorsDialog::EmulatorsDialog(vector<array<string, 3>> *vers, QNetworkAccessManager *manager, QWidget *parent) : QDialog(parent), process(new QProcess())
{
    setWindowTitle("Emulators");

    mainLayout = new QVBoxLayout(this);
    for (Emulator *emu : EMULATORS.ALL)
    {
        array<string, 3> ver;
        for (array<string, 3> ver1 : *vers) { if (ver1[0] == emu->getId()) {ver = ver1;} }
        auto *emuOptions = new EmuOptions(emu, ver, process, manager, this);
        emulators.push_back(emuOptions);
        mainLayout->addWidget(emuOptions);
    }
}

EmulatorsDialog::~EmulatorsDialog()
{
    delete mainLayout;
    delete process;
    for (EmuOptions *emuOptions : emulators) { delete emuOptions; }
}

EmuOptions::EmuOptions(Emulator *emulator, array<string, 3> ver, QProcess *process, QNetworkAccessManager *manager, QWidget *parent) : QWidget(parent), emulator(emulator), layout(new QGridLayout(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    nameLabel = new QLabel(emulator->getId().c_str(), this);
    verLabel = new QLabel(("current / latest : " + ver[1] + " / " + ver[2]).c_str(), this);
    updateButton = new QPushButton("Backup and Update", this);
    if (ver[1] == ver[2]) {updateButton->setDisabled(true);}
    updateButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(updateButton, &QPushButton::clicked, [=]() {
        updateButton->setDisabled(true);
        emulator->backup_update(parent, process, manager, [=]() {QMessageBox::information(parent, tr("Info"), tr("Update Done"));});
    });
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(verLabel, 1, 0);
    layout->addWidget(updateButton, 0, 1, Qt::AlignRight);
}

EmuOptions::~EmuOptions()
{
    delete layout;
    delete nameLabel;
    delete verLabel;
    delete updateButton;
}