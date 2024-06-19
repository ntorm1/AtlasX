#include "qaction.h"
#include "qicon.h"
#include "qlabel.h"
#include "qmessagebox.h"
#include "qstring.h"
#include "qtoolbar.h"

#include "AtlasX.h"
#include "atlasx/atlasx_impl.hpp"
#include "ui_AtlasX.h"

#include "workspace/AtlasXWorkspacePopups.hpp"

static QIcon svgIcon(const QString &File) {
  QIcon SvgIcon(File);
  SvgIcon.addPixmap(SvgIcon.pixmap(50));
  return SvgIcon;
}

struct AtlasXComponents {
  QLabel *workspace_label;

  AtlasXComponents(QWidget *parent) { workspace_label = new QLabel(parent); }
};

namespace AtlasX {

//============================================================================
AtlasXApp::AtlasXApp(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AtlasXClass()) {
  ui->setupUi(this);
  impl = std::make_unique<AtlasXImpl>(*this);
  components = std::make_unique<AtlasXComponents>(this);
  this->initToolbar();
  this->initBottomToolbar();
}

//============================================================================
AtlasXApp::~AtlasXApp() { delete ui; }

//============================================================================
void AtlasXApp::initToolbar() noexcept {
  auto tool_bar = new QToolBar(this);

  auto a = new QAction("Envs", tool_bar);
  a->setProperty("Floating", true);
  a->setToolTip("Load Workspace");
  a->setIcon(svgIcon("./ui/static/icons/workspace.png"));
  connect(a, &QAction::triggered, this, &AtlasXApp::onLoadWorkSpace);
  tool_bar->addAction(a);

  a = new QAction("New Env", tool_bar);
  a->setProperty("Floating", true);
  a->setToolTip("New Workspace");
  a->setIcon(svgIcon("./ui/static/icons/filenew.png"));
  connect(a, &QAction::triggered, this, &AtlasXApp::onNewWorkSpace);
  tool_bar->addAction(a);

  a = new QAction("Save Env", tool_bar);
  a->setProperty("Floating", true);
  a->setToolTip("Save Workspace");
  a->setIcon(svgIcon("./ui/static/icons/save.png"));
  connect(a, &QAction::triggered, this, &AtlasXApp::onSaveWorkSpace);
  tool_bar->addAction(a);

  auto separator = new QAction(tool_bar);
  separator->setSeparator(true);
  tool_bar->addAction(separator);

  a = new QAction("Build", tool_bar);
  a->setProperty("Floating", true);
  a->setToolTip("Build state");
  a->setIcon(svgIcon("./ui/static/icons/build.png"));
  connect(a, &QAction::triggered, this, &AtlasXApp::onBuild);
  tool_bar->addAction(a);

  this->addToolBar(Qt::TopToolBarArea, tool_bar);
}

//============================================================================
void AtlasXApp::initBottomToolbar() noexcept {
  auto tool_bar = new QToolBar(this);
  auto workspace = this->impl->getWorkspaceName();
  components->workspace_label =
      new QLabel("Workspace: " + QString::fromStdString(workspace));
  tool_bar->addWidget(components->workspace_label);
  this->addToolBar(Qt::BottomToolBarArea, tool_bar);
}

//============================================================================
void AtlasXApp::onBuild() noexcept {}

//============================================================================
void AtlasXApp::onLoadWorkSpace() noexcept {
  auto workspaces = impl->loadWorkspaces();
  if (workspaces.empty()) {
    auto msg = "No workspaces found";
    ATLASX_ERROR_POPUP(msg);
    return;
  }
  auto workspace_loader =
      std::make_unique<AtlasXWorkspaceLoader>(workspaces, this);
  if (workspace_loader->exec() == QDialog::Accepted) {
    ATLASX_DEBUG("Workspace loaded");
    auto res = impl->loadWorkspace(workspace_loader->getWorkspaceName());
    if (!res) {
      auto msg =
          std::format("Failed to load workspace: {}", res.error().what());
      ATLASX_ERROR_POPUP(msg);
    }
  } else {
    ATLASX_DEBUG("Workspace load canceled");
  }
  components->workspace_label->setText(
      "Workspace: " + QString::fromStdString(impl->getWorkspaceName()));
}

//============================================================================
void AtlasXApp::onSaveWorkSpace() noexcept {
  ATLASX_DEBUG("Save workspace");
  auto res = impl->saveWorkspace();
  if (!res) {
    auto msg = std::format("Failed to save workspace: {}", res.error().what());
    ATLASX_ERROR_POPUP(msg);
  }
  ATLASX_DEBUG("Workspace saved");
}

//============================================================================
void AtlasXApp::onNewWorkSpace() noexcept {
  ATLASX_DEBUG("New workspace");

  auto new_workspace = std::make_unique<AtlasXWorkspaceBuilder>(this);
  if (new_workspace->exec() == QDialog::Accepted) {
    ATLASX_DEBUG("New workspace accepted");
    auto res = impl->buildNewWorkspace(new_workspace->getWorkspaceName());
    if (!res) {
      auto msg =
          std::format("Failed to build new workspace: {}", res.error().what());
      ATLASX_ERROR_POPUP(msg);
    }
  } else {
    ATLASX_DEBUG("New workspace canceled");
  }
}

} // namespace AtlasX