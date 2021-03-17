#include "copyworker.h"


void CopyWorker::copy(std::string ob, std::string loc)
{
    std::error_code ec;

    if (std::filesystem::exists(loc + "\\Players")) {
        std::filesystem::remove_all(loc + "\\Players");
        std::filesystem::create_directory(loc + "\\Players");
    } else {
        std::filesystem::create_directory(loc + "\\Players");
    }
    if (std::filesystem::exists(loc + "\\Worlds")) {
        std::filesystem::remove_all(loc + "\\Worlds");
        std::filesystem::create_directory(loc + "\\Worlds");
    } else {
        std::filesystem::create_directory(loc + "\\Worlds");
    }

    std::filesystem::copy(ob + "\\Players", loc + "\\Players", std::filesystem::copy_options::recursive, ec);

    if (ec) {
        QListWidgetItem message(QString::fromStdString(ec.message()));
        message.setForeground(QColor("Red"));
        emit emitMessage(message);
        ec.clear();
    } else {
        QListWidgetItem message("Successfully Transferred Players");
        message.setForeground(QColor("Green"));
        emit emitMessage(message);
    }

    std::filesystem::copy(ob + "\\Worlds", loc + "\\Worlds", std::filesystem::copy_options::recursive, ec);

    if (ec) {
        QListWidgetItem message(QString::fromStdString(ec.message()));
        message.setForeground(QColor("Red"));
        emit emitMessage(message);
        ec.clear();
    } else {
        QListWidgetItem message("Successfully Transferred Worlds");
        message.setForeground(QColor("Green"));
        emit emitMessage(message);
    }
}
