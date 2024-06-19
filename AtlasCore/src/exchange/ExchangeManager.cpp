#include "pch.h"

#include "exchange/Exchange.hpp"
#include "exchange/ExchangeManager.hpp"

#include "standard/AtlasArray.hpp"

BEGIN_ATLAS_NAMESPACE

struct ExchangeMapImpl {
  HashMap<String, size_t> exchange_id_map;
  HashMap<size_t, String> asset_map;
  Vector<SharedPtr<Exchange>> exchanges;
  Vector<Int64> timestamps;
  Int64 global_time;
  size_t current_index = 0;
};

ExchangeManager::ExchangeManager() noexcept {
  m_impl = std::make_unique<ExchangeMapImpl>();
}

ExchangeManager::~ExchangeManager() noexcept {}

//================================================================================
Vector<SharedPtr<const Exchange>>
Atlas::ExchangeManager::getExchanges() const noexcept {
  Vector<SharedPtr<const Exchange>> exchange_ptrs;
	for (auto const &exchange : m_impl->exchanges) {
		exchange_ptrs.push_back(exchange);
	}
	return exchange_ptrs;
}

void ExchangeManager::build() noexcept {}

void ExchangeManager::reset() noexcept {}

void ExchangeManager::step() noexcept {}

void ExchangeManager::cleanup() noexcept {}

//================================================================================
Result<SharedPtr<Exchange>>
ExchangeManager::addExchange(String name, String source) noexcept {
  EXPECT_FALSE(m_impl->exchange_id_map.contains(name),
               "Exchange with name already exists");
  auto exchange = std::make_shared<Exchange>(std::move(name), std::move(source),
                                             m_impl->exchanges.size());
  auto CONCAT(res, _opt) = exchange->init();
  if (!CONCAT(res, _opt)) {
    return Err<Atlas::AtlasException>(CONCAT(res, _opt).error());
  };
  EXPECT_TRUE(res_val, exchange->validate());
  EXPECT_TRUE(res_build, exchange->build());
  SAFE_MAP_INSERT(m_impl->exchange_id_map, exchange->getName(),
                  m_impl->exchanges.size());
  m_impl->exchanges.push_back(std::move(exchange));

  // copy over exchange's asset map and set the exchange's index offset equal to
  // the asset map size
  auto &exchange_ptr = m_impl->exchanges.back();
  exchange_ptr->setExchangeOffset(m_impl->asset_map.size());
  for (auto const &asset : exchange_ptr->getAssetMap()) {
    m_impl->asset_map[asset.second] = asset.first;
  }
  return exchange_ptr;
}

//================================================================================
Result<SharedPtr<Exchange>> ExchangeManager::getExchange(String name) noexcept {
  if (!m_impl->exchange_id_map.contains(name)) {
    return Err<Atlas::AtlasException>("Exchange with name does not exist");
  }
  return m_impl->exchanges[m_impl->exchange_id_map[name]];
}

//================================================================================
json Atlas::ExchangeManager::getExchangeManagerJson() const noexcept {
  json j;
  j["exchanges"] = {};
  for (auto const &exchange : m_impl->exchanges) {
    j["exchanges"][exchange->getName()] = exchange->getExchangeJson();
  }
  return j;
}

//================================================================================
Result<UniquePtr<ExchangeManager>>
Atlas::ExchangeManager::fromJson(json const &j) noexcept {
  if (!j.contains("exchanges")) {
    return Err<Atlas::AtlasException>("Json does not contain exchanges");
  }
  UniquePtr<ExchangeManager> exchange_manager =
      std::make_unique<ExchangeManager>();
  for (const auto &[name, exchange_json] : j["exchanges"].items()) {
    if (!exchange_json.contains("source")) {
			return Err<Atlas::AtlasException>("Exchange does not contain source");
		}
    auto value = exchange_json["source"].get<String>();
    auto res = exchange_manager->addExchange(name, value);
    if (!res) {
      return Err<Atlas::AtlasException>(res.error());
    }
  }
  return exchange_manager;
}

END_ATLAS_NAMESPACE