#include "browsermainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BrowserMainWindow w;
    w.show();

    return a.exec();
}
