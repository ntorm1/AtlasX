#pragma once
#include "standard/AtlasTypes.hpp"

namespace Atlas {

struct ExchangeMapImpl;

class ExchangeManager {
  friend class AtlasManager;

public:
  ExchangeManager() noexcept;
  ~ExchangeManager() noexcept;

  Vector<SharedPtr<const Exchange>> getExchanges() const noexcept;


protected:
  Result<SharedPtr<Exchange>> addExchange(String name, String source) noexcept;
  Result<SharedPtr<Exchange>> getExchange(String name) noexcept;
  json getExchangeManagerJson() const noexcept;
  static Result<UniquePtr<ExchangeManager>> fromJson(json const &j) noexcept;

private:
  UniquePtr<ExchangeMapImpl> m_impl;

  void build() noexcept;
  void reset() noexcept;
  void step() noexcept;
  void cleanup() noexcept;
};

} // namespace Atlas