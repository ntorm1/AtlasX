#include "stdafx.h"
#include "AtlasX.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AtlasX w;
    w.show();
    return a.exec();
}
