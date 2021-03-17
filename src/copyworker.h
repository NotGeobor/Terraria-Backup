#ifndef COPYWORKER_H
#define COPYWORKER_H

#include <filesystem>
#include <QListWidgetItem>
#include <QObject>


class CopyWorker : public QObject
{
    Q_OBJECT

public slots:
    void copy(std::string ob, std::string loc);

signals:
    void emitMessage(QListWidgetItem message);
};

#endif // COPYWORKER_H
