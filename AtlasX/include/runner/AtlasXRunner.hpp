#pragma once

#include "atlasx/atlasx_manager.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class AtlasXRunnerClass;
}; // namespace Ui
QT_END_NAMESPACE

namespace AtlasX {

class AtlasXRunManager : public AtlasXManager {
public:
  AtlasXRunManager(String manager_id, AtlasXImpl &app_impl, QWidget *parent);
  ~AtlasXRunManager();

  AtlasXManagerType type() const noexcept override {
    return AtlasXManagerType::AtlasXRunManager;
  }

private:
  Ui::AtlasXRunnerClass *ui;
  void initUI() noexcept;
  void initToolbars() noexcept;
  void initCommandBar();
  void initSignals() noexcept;
};
} // namespace AtlasX