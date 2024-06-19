#include "runner/AtlasXRunner.hpp"

#include "ui_AtlasXRunner.h"

namespace AtlasX {

AtlasXRunManager::AtlasXRunManager(String manager_id, AtlasXImpl &app_impl,
                                   QWidget *parent)
    : AtlasXManager(manager_id, app_impl, parent),
      ui(new Ui::AtlasXRunnerClass) {
  ui->setupUi(this);
  initUI();
  initToolbars();
  initSignals();
}

AtlasXRunManager::~AtlasXRunManager() { delete ui; }

void AtlasXRunManager::initUI() noexcept {
  // Initialize the user interface components here
}

void AtlasXRunManager::initToolbars() noexcept { initCommandBar(); }

void AtlasXRunManager::initCommandBar() {

  QAction *a = new QAction("Settings", ui->top_toolbar);
  a->setProperty("Floating", true);
  a->setToolTip("Open Settings Window");
  a->setIcon(svgIcon("./ui/static/icons/settings.png"));
  ui->top_toolbar->addAction(a);

  a = new QAction("Link", ui->top_toolbar);
  a->setProperty("Floating", true);
  a->setIcon(svgIcon("./ui/static/icons/link.png"));
  ui->top_toolbar->addAction(a);

  auto sepertor = new QAction(ui->top_toolbar);
  sepertor->setSeparator(true);
  ui->top_toolbar->addAction(sepertor);

  a = new QAction("Next", ui->top_toolbar);
  a->setProperty("Floating", false);
  a->setProperty("Tabbed", true);
  a->setIcon(svgIcon("./ui/static/icons/next.png"));
  ui->top_toolbar->addAction(a);

  a = new QAction("Run", ui->top_toolbar);
  a->setProperty("Floating", false);
  a->setProperty("Tabbed", true);
  a->setIcon(svgIcon("./ui/static/icons/run.png"));
  ui->top_toolbar->addAction(a);

  a = new QAction("Reset", ui->top_toolbar);
  a->setProperty("Floating", false);
  a->setProperty("Tabbed", true);
  a->setIcon(svgIcon("./ui/static/icons/reset.png"));
  ui->top_toolbar->addAction(a);
}

void AtlasXRunManager::initSignals() noexcept {
  // Connect the signals and slots here
}

} // namespace AtlasX