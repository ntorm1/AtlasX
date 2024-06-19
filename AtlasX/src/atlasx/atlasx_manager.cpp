#include "atlasx/atlasx_manager.hpp"

namespace AtlasX {

struct AtlasXManagerImpl {
  AtlasXImpl &app_impl;
  String manager_id;
};

AtlasXManager::AtlasXManager(String manager_id, AtlasXImpl &app_impl,
                             QWidget *parent)
    : QMainWindow(parent) {
  m_impl = std::make_unique<AtlasXManagerImpl>(app_impl, manager_id);
}

AtlasXManager::~AtlasXManager() {}

String const &AtlasXManager::managerId() const noexcept {
  return m_impl->manager_id;
}

AtlasXImpl &AtlasXManager::appImpl() noexcept { return m_impl->app_impl; }

} // namespace AtlasX