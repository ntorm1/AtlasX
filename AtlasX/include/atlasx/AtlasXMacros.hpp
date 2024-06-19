#pragma once
#include <qmessagebox.h>

#define ATLASX_ERROR_POPUP(errorMessage)                                         \
  do {                                                                         \
    QMessageBox msgBox;                                                        \
    msgBox.setIcon(QMessageBox::Critical);                                     \
    ATLASX_CRITICAL("{}", msg);                                                \
    msgBox.setText(QString::fromStdString(errorMessage));											 \
    msgBox.setWindowTitle("Error"); 										 \
		msgBox.setStandardButtons(QMessageBox::Ok);                                \
		msgBox.exec();                                                             \
	} while (0)
