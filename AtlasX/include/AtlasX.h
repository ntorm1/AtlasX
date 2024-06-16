#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AtlasX.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AtlasXClass; };
QT_END_NAMESPACE

class AtlasX : public QMainWindow
{
    Q_OBJECT

public:
    AtlasX(QWidget *parent = nullptr);
    ~AtlasX();

private:
    Ui::AtlasXClass *ui;
};
