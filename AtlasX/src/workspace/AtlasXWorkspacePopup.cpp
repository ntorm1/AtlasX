#include "workspace/AtlasXWorkspacePopups.hpp"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

BEGIN_NAMESPACE_ATLASX

AtlasXWorkspaceBuilder::AtlasXWorkspaceBuilder(QWidget *parent)
    : QDialog(parent) {

  // Create UI elements
  QLabel *label = new QLabel("Enter workspace name:", this);
  QLineEdit *textEdit = new QLineEdit(this);
  QPushButton *confirmButton = new QPushButton("Confirm", this);
  QPushButton *cancelButton = new QPushButton("Cancel", this);

  // Create layouts
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  QHBoxLayout *inputLayout = new QHBoxLayout();
  QHBoxLayout *buttonLayout = new QHBoxLayout();

  // Add widgets to layouts
  inputLayout->addWidget(label);
  inputLayout->addWidget(textEdit);

  buttonLayout->addWidget(confirmButton);
  buttonLayout->addWidget(cancelButton);

  mainLayout->addLayout(inputLayout);
  mainLayout->addLayout(buttonLayout);

  // Connect signals to slots
  connect(confirmButton, &QPushButton::clicked, this,
          &AtlasXWorkspaceBuilder::onConfirm);
  connect(cancelButton, &QPushButton::clicked, this,
          &AtlasXWorkspaceBuilder::onCancel);

  // Set layout to dialog
  setLayout(mainLayout);

  // Set dialog title
  setWindowTitle("Load Workspace");
}
void AtlasXWorkspaceBuilder::onConfirm() {
  QLineEdit *textEdit = findChild<QLineEdit *>();
  if (textEdit) {
    QString text = textEdit->text();
    qDebug() << "Confirm clicked, text:" << text;
    accept(); // Close the dialog with an accept code
  }
}

void AtlasXWorkspaceBuilder::onCancel() {
  qDebug() << "Cancel clicked";
  reject(); // Close the dialog with a reject code
}

String AtlasXWorkspaceBuilder::getWorkspaceName() const noexcept {
  QLineEdit *textEdit = findChild<QLineEdit *>();
  return textEdit ? textEdit->text().toStdString() : "";
}

AtlasXWorkspaceLoader::AtlasXWorkspaceLoader(Vector<String> const &workspaces,
                                             QWidget *parent)
    : QDialog(parent) {

  // Create UI elements
  QLabel *label = new QLabel("Select workspace:", this);
  workspaceComboBox = new QComboBox(this);
  QVector<QString> workspacesQStr;
  for (auto const &workspace : workspaces) {
		workspacesQStr.push_back(QString::fromStdString(workspace));
	}
  workspaceComboBox->addItems(
            workspacesQStr.toList()); // Add workspaces to the combo box

  confirmButton = new QPushButton("Confirm", this);
  QPushButton *cancelButton = new QPushButton("Cancel", this);

  // Create layouts
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  QHBoxLayout *inputLayout = new QHBoxLayout();
  QHBoxLayout *buttonLayout = new QHBoxLayout();

  // Add widgets to layouts
  inputLayout->addWidget(label);
  inputLayout->addWidget(workspaceComboBox);

  buttonLayout->addWidget(confirmButton);
  buttonLayout->addWidget(cancelButton);

  mainLayout->addLayout(inputLayout);
  mainLayout->addLayout(buttonLayout);

  // Connect signals to slots
  connect(confirmButton, &QPushButton::clicked, this,
          &AtlasXWorkspaceLoader::onConfirm);
  connect(cancelButton, &QPushButton::clicked, this,
          &AtlasXWorkspaceLoader::onCancel);

  // Set layout to dialog
  setLayout(mainLayout);

  // Set dialog title
  setWindowTitle("Load Workspace");
}

void AtlasXWorkspaceLoader::onConfirm() {
  QString selectedWorkspace = workspaceComboBox->currentText();
  qDebug() << "Confirm clicked, selected workspace:" << selectedWorkspace;
  accept(); // Close the dialog with an accept code
}

void AtlasXWorkspaceLoader::onCancel() {
  qDebug() << "Cancel clicked";
  reject(); // Close the dialog with a reject code
}

END_NAMESPACE_ATLASX