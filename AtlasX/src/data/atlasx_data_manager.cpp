#include "qstandarditemmodel.h"

#include "ui_AtlasXNewExchange.h"
#include "ui_atlasx_data_manager.h"

#include "data/atlasx_data_manager.hpp"
#include "exchange/Exchange.hpp"
#include "manager/AtlasManager.hpp"
#include "standard/AtlasTime.hpp"

namespace AtlasX {
AtlasXDataManager::AtlasXDataManager(String manager_id, AtlasXImpl &app_impl,
                                     QWidget *parent)
    : AtlasXManager(manager_id, app_impl, parent),
      ui(new Ui::AtlasXDataClass()) {
  ui->setupUi(this);
  ui->splitter->setStretchFactor(0, 1);
  ui->splitter->setStretchFactor(1, 1);
  this->initUI();
}

//==============================================================================
AtlasXDataManager::~AtlasXDataManager() {}

//==============================================================================
void AtlasXDataManager::initUI() noexcept {
  this->initToolbars();
  this->initSignals();
}

//==============================================================================
void AtlasXDataManager::initToolbars() noexcept {
  ui->actionNew_Exchange->setIcon(svgIcon("./ui/static/icons/add.png"));
  connect(ui->actionNew_Exchange, &QAction::triggered, this,
          &AtlasXDataManager::onNewExchange);

  auto const exchanges = appImpl().getAtlasManager()->getExchanges();
  QVector<QString> exchange_names;
  for (auto const &exchange : exchanges) {
    exchange_names.push_back(QString::fromStdString(exchange->getName()));
  }
  m_exchange_combo_box = new QComboBox(this);
  m_exchange_combo_box->addItems(exchange_names);
  ui->exchange_top_toolbar->addWidget(new QLabel("Exchange: ", this));
  ui->exchange_top_toolbar->addWidget(m_exchange_combo_box);
  connect(m_exchange_combo_box, &QComboBox::currentTextChanged, this,
          &AtlasXDataManager::onExchangeSelected);
  if (!exchange_names.empty()) {
    this->initExchangeUI(exchange_names[0].toStdString());
  }
}

//==============================================================================
void AtlasXDataManager::initSignals() noexcept {
  connect(ui->asset_combobox, &QComboBox::currentTextChanged, this,
          &AtlasXDataManager::onAssetSelected);
}

//==============================================================================
void AtlasXDataManager::onExchangeSelected() noexcept {
  auto exchange_name = m_exchange_combo_box->currentText().toStdString();
  auto exchange = appImpl().getAtlasManager()->getExchange(exchange_name);
  if (!exchange) {
    auto msg =
        std::format("Failed to get exchange: {}", exchange.error().what());
    ATLASX_CRITICAL("{}", msg);
    ATLASX_ERROR_POPUP(msg);
    return;
  }
  ATLASX_DEBUG("Selected Exchange: {}", m_selected_exchange.value());
  this->initExchangeUI(exchange_name);
}

//==============================================================================
void AtlasXDataManager::onNewExchange() noexcept {
  ATLASX_DEBUG("New Exchange");
  QDialog *dialog = new QDialog(this);
  auto new_exchange = std::make_unique<Ui::AtlasXNewExchangeClass>();
  new_exchange->setupUi(dialog);
  if (dialog->exec() == QDialog::Accepted) {
    ATLASX_DEBUG("New Exchange Accepted");
    auto exchange_name = new_exchange->exchange_name_edit->text().toStdString();
    auto exchange_source =
        new_exchange->exchange_source_edit->text().toStdString();
    auto res = appImpl().newExchange(exchange_name, exchange_source);
    if (!res) {
      auto msg =
          std::format("Failed to create new exchange: {}", res.error().what());
      ATLASX_CRITICAL("{}", msg);
      ATLASX_ERROR_POPUP(msg);
    } else {
      initExchangeUI(exchange_name);
    }
  }
}

//==============================================================================
void AtlasXDataManager::initExchangeUI(String selected_exchange) noexcept {
  m_selected_exchange = selected_exchange;
  auto const &exchange =
      appImpl().getAtlasManager()->getExchange(selected_exchange).value();

  auto const &asset_map = exchange->getAssetMap();
  QVector<QString> asset_names;
  for (auto const &[asset, id] : asset_map) {
    asset_names.push_back(QString::fromStdString(asset));
  }
  ui->asset_combobox->clear();
  ui->asset_combobox->addItems(asset_names);
  if (asset_names.size() > 0) {
    this->onAssetSelected();
  }
}

//==============================================================================
void AtlasXDataManager::onAssetSelected() noexcept {
  this->buildAssetDataTable();
}

//==============================================================================
void AtlasXDataManager::buildAssetDataTable() noexcept {
  auto const &exchange = appImpl()
                             .getAtlasManager()
                             ->getExchange(m_selected_exchange.value())
                             .value();
  auto selected_asset = ui->asset_combobox->currentText().toStdString();
  auto asset_idx = exchange->getAssetIndex(selected_asset);
  assert(asset_idx.has_value());

  auto const &headers = exchange->getHeaders();
  auto const asset_slice = exchange->getAssetSlice(asset_idx.value());
  auto const &timestamps = exchange->getDatetimeIndex();
  auto column_count = headers.size();
  auto row_count = asset_slice.size() / column_count;

  auto table_model = new QStandardItemModel(this);
  QVector<QString> header_names;
  header_names.push_back("Datetime");
  for (auto const &[header, idx] : headers) {
    header_names.push_back(QString::fromStdString(header));
  }
  table_model->setHorizontalHeaderLabels(header_names);
  for (int row = 0; row < row_count; ++row) {
    QList<QStandardItem *> row_items;
    auto datetime__str = Atlas::convertNanosecondsToTime(timestamps[row]);
    row_items.append(new QStandardItem(QString::fromStdString(datetime__str)));
    for (int col = 0; col < column_count; ++col) {
      row_items.append(new QStandardItem(
          QString::number(asset_slice[row * column_count + col])));
    }
    table_model->appendRow(row_items);
  }
  ui->asset_table_view->reset();
  ui->asset_table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->asset_table_view->setModel(table_model);
  ui->asset_table_view->resizeColumnsToContents();
}

} // namespace AtlasX