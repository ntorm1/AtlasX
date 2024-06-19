#include "qapplication.h"
#include <filesystem>
#include <fstream>

#include "AtlasX.h"
#include "atlasx/atlasx_impl.hpp"
#include "data/atlasx_data_manager.hpp"
#include "runner/AtlasXRunner.hpp"

#include "manager/AtlasManager.hpp"

namespace fs = std::filesystem;
using namespace ads;

static QIcon svgIcon(const QString &File) {
  QIcon SvgIcon(File);
  SvgIcon.addPixmap(SvgIcon.pixmap(92));
  return SvgIcon;
}

namespace AtlasX {

struct AtlasXImplPrivate {
  String exe_path;
  String workspace;
  fs::path env_path;
  UniquePtr<Atlas::AtlasManager> atlas_manager;

  //============================================================================
  static Result<UniquePtr<AtlasXImplPrivate>> create() noexcept {
    auto impl = std::make_unique<AtlasXImplPrivate>();
    auto res = impl->loadWorkspace("default");
    if (!res) {
      return Err(res.error());
    }
    return impl;
  }

  //============================================================================
  Result<bool> loadWorkspace(String const &new_workspace) noexcept {
    workspace = new_workspace;
    env_path = envBase() / workspace;
    auto workspace_json = env_path / "workspace.json";
    if (!fs::exists(workspace_json)) {
      atlas_manager = std::make_unique<Atlas::AtlasManager>();
      return true;
    }
    auto res = Atlas::AtlasManager::fromJson(workspace_json.string());
    if (!res) {
      return Err(res.error());
    }
    atlas_manager = std::move(res.value());
    return true;
  }

  //============================================================================
  fs::path envBase() const noexcept {
    auto parent_dir = fs::path(exe_path).parent_path();
    return parent_dir / "envs";
  }

  //============================================================================
  AtlasXImplPrivate() noexcept {
    exe_path = QCoreApplication::applicationDirPath().toStdString();
    if (!fs::exists(envBase())) {
      fs::create_directory(envBase());
    }
    env_path = envBase() / "default";
    workspace = "default";
    atlas_manager = std::make_unique<Atlas::AtlasManager>();
  }
};

//============================================================================
AtlasXImpl::AtlasXImpl(AtlasXApp &app) : m_app(app) {
  CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
  CDockManager::setConfigFlag(CDockManager::AllTabsHaveCloseButton, true);
  CDockManager::setConfigFlag(CDockManager::DragPreviewIsDynamic, true);
  CDockManager::setConfigFlag(CDockManager::DragPreviewHasWindowFrame, false);
  CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
  m_dock_manager = std::make_unique<ads::CDockManager>(&m_app);

  auto res = AtlasXImplPrivate::create();
  if (!res) {
    auto msg =
        std::format("Failed to create AtlasXImpl: {}", res.error().what());
    ATLASX_CRITICAL("{}", msg);
    assert(false); // TODO: Handle error
  }
  m_impl = std::move(res.value());

  this->initState();
  this->initUI();
}

//============================================================================
AtlasXImpl::~AtlasXImpl() {}

//============================================================================
String const &AtlasXImpl::getWorkspaceName() const noexcept {
  return m_impl->workspace;
}

//============================================================================
Result<bool> AtlasXImpl::newExchange(String name, String source) noexcept {
  auto res = m_impl->atlas_manager->addExchange(name, source);
  if (!res) {
    return Err(res.error());
  }
  return true;
}

UniquePtr<Atlas::AtlasManager> const &
AtlasXImpl::getAtlasManager() const noexcept {
  return m_impl->atlas_manager;

}

//============================================================================
void AtlasXImpl::initState() noexcept {
  ATLASX_DEBUG("Initializing AtlasX state");
  if (!fs::exists(m_impl->env_path)) {
    auto res = this->buildNewWorkspace("default");
    assert(res); // TODO: Handle error
  }
}

//============================================================================
void AtlasXImpl::initUI() noexcept {
  ATLASX_DEBUG("Initializing AtlasX state");
  auto data_manager = std::make_unique<AtlasXDataManager>(
      this->nextComponentId(AtlasXManagerType::AtlasXDataManager), *this,
      &m_app);
  auto dock = new ads::CDockWidget(
      QString::fromStdString(data_manager->managerId()), &m_app);
  dock->setIcon(svgIcon("./ui/static/icons/exchange.png"));
  dock->setWidget(data_manager.get());

  auto dock_area =
      m_dock_manager->addDockWidget(ads::RightDockWidgetArea, dock);

  m_managers[data_manager->managerId()] = std::move(data_manager);


  auto runner = std::make_unique<AtlasXRunManager>(
      this->nextComponentId(AtlasXManagerType::AtlasXRunManager), *this,
      &m_app);
  dock = new ads::CDockWidget(
			QString::fromStdString(runner->managerId()), &m_app);
  dock->setIcon(svgIcon("./ui/static/icons/run.png"));
  dock->setWidget(runner.get());
  dock_area = m_dock_manager->addDockWidget(ads::RightDockWidgetArea, dock);
  m_managers[runner->managerId()] = std::move(runner);

  ATLASX_DEBUG("AtlasX state initialized");
}

//============================================================================
String AtlasXImpl::nextComponentId(AtlasXManagerType comp_type) const noexcept {
  int comp_count = 0;

  for (auto const &[id, comp] : this->m_managers) {
    if (comp->type() == comp_type) {
      comp_count++;
    }
  }
  return std::format("{} {}", componentTypeToString(comp_type), comp_count);
}

Result<bool> AtlasXImpl::buildNewWorkspace(String workspace_name) noexcept {
  ATLASX_DEBUG("Building new workspace");
  try {
    if (workspace_name == "") {
      return Err("Workspace name cannot be empty");
    }

    auto workspace_folder = m_impl->envBase() / workspace_name;
    if (fs::exists(workspace_folder)) {
      return Err("Workspace already exists");
    } else {
      fs::create_directory(workspace_folder);
    }
    auto workspace_json = workspace_folder / "workspace.json";
    auto res = m_impl->atlas_manager->toJson(workspace_json.string());
    if (!res) {
      return Err(res.error());
    }
    return true;
  } catch (std::exception &e) {
    auto msg = std::format("Failed to build new workspace: {}", e.what());
    ATLASX_CRITICAL("{}", msg);
    return Err(msg);
  }
}

//============================================================================
Result<bool> AtlasXImpl::loadWorkspace(String workspace_name) noexcept {
  auto res = m_impl->loadWorkspace(workspace_name);
  if (!res) {
    return Err(res.error());
  }
  ATLASX_DEBUG("Workspace loaded: {}", workspace_name);
  return true;
}

//============================================================================
Result<bool> AtlasXImpl::saveWorkspace() noexcept {
  auto workspace_json = m_impl->env_path / "workspace.json";
  auto res = m_impl->atlas_manager->toJson(workspace_json.string());
  if (!res) {
    return Err(res.error());
  }
  ATLASX_DEBUG("Workspace saved to: {}", workspace_json.string());
  return true;
}

//============================================================================
Vector<String> AtlasXImpl::loadWorkspaces() const noexcept {
  Vector<String> workspaces;
  for (auto &entry : fs::directory_iterator(m_impl->envBase())) {
    if (entry.is_directory()) {
      workspaces.push_back(entry.path().filename().string());
    }
  }
  return workspaces;
}

} // namespace AtlasX