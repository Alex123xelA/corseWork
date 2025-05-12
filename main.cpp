#include "TaskApp.h"
#include "UserGuide.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TaskApp w;
    UserGuide userGuide;
    userGuide.show();
    w.show();
    return a.exec();
}
