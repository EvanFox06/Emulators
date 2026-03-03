#include "addgamedialog.h"

AddGameDialog::AddGameDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Add Game");
    layout = new QGridLayout(this);

    nameLabel = new QLabel(tr("Name:"), this);
    layout->addWidget(nameLabel, 0, 0, Qt::AlignRight);

    nameInput = new QLineEdit(this);
    nameInput->setFixedWidth(200);
    layout->addWidget(nameInput, 0, 1);

    imageUrlLabel = new QLabel(tr("Image URL:"), this);
    layout->addWidget(imageUrlLabel, 1, 0, Qt::AlignRight);

    imageUrlInput = new QLineEdit(this);
    imageUrlInput->setFixedWidth(200);
    layout->addWidget(imageUrlInput, 1, 1);

    imageFetchButton = new QPushButton(tr("Fetch Image"), this);
    connect(imageFetchButton, &QPushButton::pressed, this, &AddGameDialog::fetchImage);
    imageFetchButton->setFixedWidth(200);
    layout->addWidget(imageFetchButton, 2, 1);

    nameLabel = new QLabel(tr("File:"), this);
    layout->addWidget(nameLabel, 3, 0, Qt::AlignRight);

    chooseFileButton = new QPushButton(tr("Choose File"), this);
    connect(chooseFileButton, &QPushButton::pressed, this, &AddGameDialog::openFileDialog);
    chooseFileButton->setFixedWidth(200);
    layout->addWidget(chooseFileButton, 3, 1);

    image = QPixmap(1, 1);
    imagePreview = new QLabel(this);
    imagePreview->setPixmap(image);
    imagePreview->setScaledContents(true);
    imagePreview->setFixedSize(150, 150);
    layout->addWidget(imagePreview, 0, 2, 4, 1);

    addGameButton = new QPushButton(tr("Add Game"), this);
    connect(addGameButton, &QPushButton::pressed, this, &AddGameDialog::addGame);
    layout->addWidget(addGameButton, 4, 0, 1, 3);
}

void AddGameDialog::openFileDialog()
{
    fileName = QFileDialog::getOpenFileName(this,
        tr("Choose File"),
        QDir::home().filePath("Downloads"),
        tr("Game ROMs (*.nds *.gba *.rvz *.cci)")
    );
    chooseFileButton->setText(fileName);
}

void AddGameDialog::fetchImage()
{
    imageFetchButton->setDisabled(true);
    networkManager.get(QNetworkRequest(QUrl(imageUrlInput->text())));
    connect(&networkManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        image.loadFromData(reply->readAll());
        imagePreview->setPixmap(image);
        imageFetchButton->setDisabled(false);
    });
}

void AddGameDialog::addGame()
{
    Emulator *emu = EMULATORS.fromExt(QFileInfo(fileName).suffix());
    QDir game_path(QString::fromStdString(emu->getPath()) + "/games/" + nameInput->text());
    if (!game_path.mkpath(".")) {qDebug() << "failed to make the directory: " + game_path.path();}
    if (!QFile::copy(fileName, game_path.filePath("game." + QString::fromStdString(emu->getExt()))))
    { qDebug() << "failed to copy file: " + fileName; }
    if (!image.save(game_path.filePath("icon.png"))) { qDebug() << "failed to save image to " + game_path.filePath("icon.png");}
    accept();
}