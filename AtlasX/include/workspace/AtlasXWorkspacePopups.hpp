#pragma once
#include "atlasx_standard.hpp"
#include <QComboBox>
#include <QDialog>

namespace AtlasX {

class AtlasXWorkspaceBuilder : public QDialog {
  Q_OBJECT

public:
  explicit AtlasXWorkspaceBuilder(QWidget *parent = nullptr);
  String getWorkspaceName() const noexcept;

private slots:
  void onConfirm();
  void onCancel();
};

class AtlasXWorkspaceLoader : public QDialog {
  Q_OBJECT
public:
  explicit AtlasXWorkspaceLoader(Vector<String> const &workspaces,
                                 QWidget *parent = nullptr);

  String getWorkspaceName() const noexcept {
    return workspaceComboBox->currentText().toStdString();
  }

private:
  QComboBox *workspaceComboBox;
  QPushButton *confirmButton;

private slots:
  void onConfirm();
  void onCancel();
};
} // namespace AtlasX
