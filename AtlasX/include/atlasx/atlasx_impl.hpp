#pragma once

#include "QtADS/src/DockAreaWidget.h"
#include "QtADS/src/DockManager.h"

#include "atlasx_standard.hpp"

using namespace Atlas;

namespace AtlasX {

struct AtlasXImplPrivate;

class AtlasXImpl {
  friend class AtlasXApp;

public:
  AtlasXImpl(AtlasXApp &app);
  ~AtlasXImpl();

  String const &getWorkspaceName() const noexcept;
  Result<bool> newExchange(String name, String source) noexcept;
  UniquePtr<Atlas::AtlasManager> const& getAtlasManager() const noexcept;

protected:
  Result<bool> buildNewWorkspace(String workspace_name) noexcept;
  Result<bool> loadWorkspace(String workspace_name) noexcept;
  Result<bool> saveWorkspace() noexcept;
  Vector<String> loadWorkspaces() const noexcept;

private:
  AtlasXApp &m_app;
  UniquePtr<AtlasXImplPrivate> m_impl;
  UniquePtr<ads::CDockManager> m_dock_manager;
  HashMap<String, UniquePtr<AtlasXManager>> m_managers;

  void initState() noexcept;
  void initUI() noexcept;

  String nextComponentId(AtlasXManagerType comp_type) const noexcept;

};

} // namespace AtlasX