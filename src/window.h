#ifndef WINDOW_H
#define WINDOW_H
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <QPushButton>
#include <QListWidget>
#include <QThread>
#include <QLabel>
#include "json.hpp"
#include "copyworker.h"
#include "path.h"


class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);

public slots:
    void backup();
    void restore();
    void uBackup();
    void uRestore();
    void tSetPath();
    void bSetPath();
    void saveJson();
    void addList(QListWidgetItem message);

signals:
    void emitPath(std::string ob, std::string loc);
    void emitTSetPath(bool &saved, std::string path);
    void emitBSetPath(bool &saved, std::string path);

private:
    QPushButton *bButton;
    QPushButton *ubButton;
    QPushButton *rButton;
    QPushButton *urButton;
    QListWidget *log;
    QLabel *tLabel;
    Path *tDir;
    QPushButton *tBrowse;
    QLabel *bLabel;
    Path *bDir;
    QPushButton *bBrowse;
    QThread copyThread;
    CopyWorker *copyWorker;

    std::string local = getenv("LOCALAPPDATA");
    std::string user = getenv("USERPROFILE");
    std::string tPath;
    std::string bPath;
    bool saved = true;

    std::vector<QListWidgetItem> messages;
};

#endif // WINDOW_H
