#include "pch.h"

#include "manager/AtlasManager.hpp"

#include "exchange/ExchangeManager.hpp"

namespace Atlas {

//============================================================================
struct AtlasManagerImpl {
  AtlasState m_state;
  UniquePtr<ExchangeManager> m_exchange_manager;

  AtlasManagerImpl() : m_state(AtlasState::INIT) {
    m_exchange_manager = std::make_unique<ExchangeManager>();
  }
};


//============================================================================
Vector<SharedPtr<const Exchange>> AtlasManager::getExchanges() const noexcept {
	return m_impl->m_exchange_manager->getExchanges();
}

//============================================================================
Result<UniquePtr<AtlasManager>>
AtlasManager::fromJson(String const &path) noexcept {
  fs::path p(path);
  if (!fs::exists(p)) {
		return Err("Path does not exist");
	}
  std::ifstream i(path);
  json j;
  i >> j;
  i.close();
  if (j.empty()) {
		return Err("Json is empty");
	}

  UniquePtr<AtlasManager> atlas_manager = std::make_unique<AtlasManager>();
  if (!j.contains("data")) {
    return Err("Json does not contain any data");
  } else {
    auto res = ExchangeManager::fromJson(j["data"]);
    if (!res) {
      return Err(res.error());
    }
    atlas_manager->m_impl->m_exchange_manager = std::move(res.value());
  }
  return atlas_manager;
}


//============================================================================
AtlasManager::AtlasManager() { m_impl = std::make_unique<AtlasManagerImpl>(); }

//============================================================================
AtlasManager::~AtlasManager() {}

//============================================================================
Result<SharedPtr<const Exchange>> AtlasManager::addExchange(String name,
                                                      String source) noexcept {
  return m_impl->m_exchange_manager->addExchange(name, source);
}

//============================================================================
Result<SharedPtr<const Exchange>> AtlasManager::getExchange(String name) const noexcept {
  return m_impl->m_exchange_manager->getExchange(name);
}

//============================================================================
Result<bool> AtlasManager::toJson(String const& path) const noexcept {
  json j;
  j["data"] = m_impl->m_exchange_manager->getExchangeManagerJson();
  try {
    fs::path p(path);
    std::ofstream o(p);
    o << std::setw(4) << j << std::endl;
    o.close();
    return true;
  }
  catch (std::exception const &e) {
		return Err(e.what());
	}
}

} // namespace Atlas
