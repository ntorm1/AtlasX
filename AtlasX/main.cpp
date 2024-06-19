#include <QtWidgets/QApplication>
#include "qicon.h"
#include "AtlasX.h"

using namespace AtlasX;


int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon("./ui/static/icons/analysis.png"));
  AtlasXApp w;
  w.resize(1400, 1000);
  w.show();
  return a.exec();
}
