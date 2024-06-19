#pragma once

#include "standard/AtlasTypes.hpp"

namespace Atlas {

enum class AtlasState { INIT = 0, BUILT = 1, RUNING = 2, FINISHED = 3 };

struct AtlasManagerImpl;

class AtlasManager {
public:
  ATLAS_API AtlasManager();
  ATLAS_API ~AtlasManager();

  ATLAS_API Result<SharedPtr<const Exchange>> addExchange(String name,
                                                   String source) noexcept;
  ATLAS_API Result<SharedPtr<const Exchange>> getExchange(String name) const noexcept;
  ATLAS_API Vector<SharedPtr<const Exchange>> getExchanges() const noexcept;

  ATLAS_API Result<bool> toJson(String const &path) const noexcept;
  ATLAS_API static Result<UniquePtr<AtlasManager>>
  fromJson(String const &path) noexcept;

private:
  UniquePtr<AtlasManagerImpl> m_impl;
};

} // namespace Atlas