#include "window.h"


Window::Window(QWidget *parent) : QWidget(parent)
{
    if (!std::filesystem::exists(local + "\\Geobor\\Terraria Backup\\config.json")) {
        if (!std::filesystem::exists(local + "\\Geobor"))
            std::filesystem::create_directory(local + "\\Geobor");
        if (!std::filesystem::exists(local + "\\Geobor\\Terraria Backup"))
            std::filesystem::create_directory(local + "\\Geobor\\Terraria Backup");

        std::ofstream wFile(local + "\\Geobor\\Terraria Backup\\config.json", std::ios::out);

        nlohmann::json newWrite =
        {
            {"Terraria Path", user + "\\Documents\\My Games\\Terraria"},
            {"Backup Path", local + "\\Geobor\\Terraria Backup"}
        };

        wFile << std::setw(4) << newWrite << '\n';
        wFile.close();
    }

    std::ifstream rFile(local + "\\Geobor\\Terraria Backup\\config.json", std::ios::in);
    nlohmann::json jsonIn = nlohmann::json::parse(rFile);
    rFile.close();

    tPath = jsonIn.at("Terraria Path");
    bPath = jsonIn.at("Backup Path");

    bButton = new QPushButton("Backup", this);
    ubButton = new QPushButton("Restore", this);
    rButton = new QPushButton("Undo Backup", this);
    urButton = new QPushButton("Undo Restore", this);
    log = new QListWidget(this);
    tLabel = new QLabel("Path to Terraria folder:", this);
    tDir = new Path(this);
    tBrowse = new QPushButton("Browse", this);
    bLabel = new QLabel("Path to backup folder:", this);
    bDir = new Path(this);
    bBrowse = new QPushButton("Browse", this);
    copyWorker = new CopyWorker;

    tDir->setText(QString::fromStdString(tPath));
    bDir->setText(QString::fromStdString(bPath));

    bButton->setGeometry(10, 10, 100, 50);
    ubButton->setGeometry(10, 70, 100, 50);
    rButton->setGeometry(210, 10, 100, 50);
    urButton->setGeometry(210, 70, 100, 50);
    log->setGeometry(10, 130, 300, 200);
    tLabel->setGeometry(10, 340, 300, 20);
    tDir->setGeometry(10, 370, 200, 30);
    tBrowse->setGeometry(220, 370, 90, 30);
    bLabel->setGeometry(10, 410, 300, 20);
    bDir->setGeometry(10, 440, 200, 30);
    bBrowse->setGeometry(220, 440, 90, 30);
    copyWorker->moveToThread(&copyThread);
    copyThread.start();

    QObject::connect(tBrowse, SIGNAL(clicked()), this, SLOT(tSetPath()));
    QObject::connect(bBrowse, SIGNAL(clicked()), this, SLOT(bSetPath()));
    QObject::connect(bButton, SIGNAL(clicked()), this, SLOT(backup()));
    QObject::connect(ubButton, SIGNAL(clicked()), this, SLOT(restore()));
    QObject::connect(rButton, SIGNAL(clicked()), this, SLOT(uBackup()));
    QObject::connect(urButton, SIGNAL(clicked()), this, SLOT(uRestore()));
    QObject::connect(this, &Window::emitPath, copyWorker, &CopyWorker::copy);
    QObject::connect(this, &Window::emitTSetPath, tDir, &Path::getPath);
    QObject::connect(copyWorker, &CopyWorker::emitMessage, this, &Window::addList);
}


void Window::backup()
{
    bPath = bDir->text().toLocal8Bit().constData();
    if (!std::filesystem::exists(bPath + "\\Backup"))
        std::filesystem::create_directory(bPath + "\\Backup");
    if (!std::filesystem::exists(bPath + "\\uBackup"))
        std::filesystem::create_directory(bPath + "\\uBackup");

    emit emitPath(bPath + "\\Backup", bPath + "\\uBackup");
    emit emitPath(tDir->text().toLocal8Bit().constData(), bPath + "\\Backup");
    if (!saved)
        saveJson();
}


void Window::restore()
{
    bPath = bDir->text().toLocal8Bit().constData();
    if (!std::filesystem::exists(bPath + "\\uRestore"))
        std::filesystem::create_directory(bPath + "\\uRestore");

    emit emitPath(tDir->text().toLocal8Bit().constData(), bPath + "\\uRestore");
    emit emitPath(bPath + "\\Backup", tDir->text().toLocal8Bit().constData());
    if (!saved)
        saveJson();
}


void Window::uBackup()
{
    bPath = bDir->text().toLocal8Bit().constData();

    emit emitPath(bPath + "\\uBackup", bPath + "\\Backup");
    if (!saved)
        saveJson();
}


void Window::uRestore()
{
    bPath = bDir->text().toLocal8Bit().constData();

    emit emitPath(bPath + "\\uRestore", tDir->text().toLocal8Bit().constData());
    if (!saved)
        saveJson();
}


void Window::tSetPath()
{
    emit emitTSetPath(saved, tDir->text().toLocal8Bit().constData());
}


void Window::bSetPath()
{
    emit emitBSetPath(saved, bDir->text().toLocal8Bit().constData());
}


void Window::saveJson()
{
    std::ofstream wFile(local + "\\Geobor\\Terraria Backup\\config.json", std::ios::out);

    nlohmann::json tempWrite =
    {
        {"Terraria Path", tDir->text().toLocal8Bit().constData()},
        {"Backup Path", bDir->text().toLocal8Bit().constData()}
    };

    wFile << std::setw(4) << tempWrite << '\n';
    wFile.close();
    saved = true;
}


void Window::addList(QListWidgetItem message)
{
    messages.push_back(message);
    for (QListWidgetItem &gamer : messages) {
        log->addItem(&gamer);
    }
}
