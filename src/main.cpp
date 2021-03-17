#include <QApplication>
#include "window.h"


int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    Window *window = new Window();

    window->setWindowTitle("Terraria Backup 1000000000");
    window->setFixedSize(320, 480);
    window->show();

    return app.exec();
}
