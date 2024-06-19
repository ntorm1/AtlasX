#pragma once
#include "qcombobox.h"

#include "atlasx/atlasx_manager.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class AtlasXDataClass;
class AtlasXNewExchangeClass;
};
QT_END_NAMESPACE

namespace AtlasX {

class AtlasXDataManager : public AtlasXManager {
public:
  AtlasXDataManager(String manager_id, AtlasXImpl &app_impl, QWidget *parent);
  ~AtlasXDataManager();

  AtlasXManagerType type() const noexcept override {
    return AtlasXManagerType::AtlasXDataManager;
  }


private:
  Ui::AtlasXDataClass *ui;
  Option<String> m_selected_exchange;

  QComboBox *m_exchange_combo_box;

  void initUI() noexcept;
  void initToolbars() noexcept;
  void initSignals() noexcept;

  void buildAssetDataTable() noexcept;

  void onNewExchange() noexcept;
  void initExchangeUI(String exchange_name) noexcept;
  void onExchangeSelected() noexcept;

private slots:
  void onAssetSelected() noexcept;

};
} // namespace AtlasX