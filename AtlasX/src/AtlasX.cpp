#include "stdafx.h"
#include "AtlasX.h"

AtlasX::AtlasX(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AtlasXClass())
{
    ui->setupUi(this);
}

AtlasX::~AtlasX()
{
    delete ui;
}
