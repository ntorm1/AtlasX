#pragma once
#include "atlasx_declare.hpp"
#include "atlasx/AtlasXMacros.hpp"
#include "standard/AtlasTypes.hpp"

#define BEGIN_NAMESPACE_ATLASX namespace AtlasX {
#define END_NAMESPACE_ATLASX }

using namespace Atlas;

BEGIN_NAMESPACE_ATLASX

enum class AtlasXManagerType { AtlasXDataManager, AtlasXWorkspaceManager, AtlasXRunManager };

static String const &componentTypeToString(AtlasXManagerType type) noexcept {
  static const HashMap<AtlasXManagerType, std::string> typeMap = {
      {AtlasXManagerType::AtlasXDataManager, "AtlasXDataManager"},
      {AtlasXManagerType::AtlasXRunManager, "AtlasXRunManager"},
    
  };
  assert(typeMap.contains(type));
  return typeMap.at(type);
}

END_NAMESPACE_ATLASX

#define QFORMAT(fmt, ...) QString::fromStdString(std::format(fmt, __VA_ARGS__))

#define ATLASX_DEBUG(fmt, ...)                                                 \
  do {                                                                         \
    qDebug() << QString::fromStdString(                                        \
        std::format("[{}:{}]: " fmt, __FILE__, __LINE__, __VA_ARGS__));        \
  } while (0)

#define ATLASX_CRITICAL(fmt, ...)                                              \
  do {                                                                         \
    qDebug() << QString::fromStdString(                                        \
        std::format("[{}:{}]: " fmt, __FILE__, __LINE__, __VA_ARGS__));        \
  } while (0)