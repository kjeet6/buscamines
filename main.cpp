#include "widget.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile stylefile(":/style/style.qss");

    stylefile.open(QFile::ReadOnly);
    QString style(stylefile.readAll());
    a.setStyleSheet(style);
    Widget w;
    w.show();

    return a.exec();
}
