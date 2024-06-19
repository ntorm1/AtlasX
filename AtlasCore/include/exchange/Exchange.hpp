
#pragma once
#include "standard/AtlasLinAlg.hpp"
#include "standard/AtlasTypes.hpp"

namespace Atlas {

struct ExchangeImpl;

class Exchange {
  friend class ExchangeManager;

private:
  UniquePtr<ExchangeImpl> m_impl;

  [[nodiscard]] Result<bool> initDir() noexcept;
  [[nodiscard]] Result<bool> init() noexcept;
  [[nodiscard]] Result<bool> validate() noexcept;
  [[nodiscard]] Result<bool> build() noexcept;
  [[nodiscard]] Option<size_t> getCloseIndex() const noexcept;

protected:
  void setExchangeOffset(size_t _offset) noexcept;
  json getExchangeJson() const noexcept;

public:
  Exchange(String name, String source, size_t id) noexcept;
  ~Exchange() noexcept;

  ATLAS_API String getName() const noexcept;
  ATLAS_API HashMap<String, size_t> const &getAssetMap() const noexcept;
  ATLAS_API HashMap<String, size_t> const &getHeaders() const noexcept;
  ATLAS_API Vector<Int64> const& getDatetimeIndex() const noexcept;
  ATLAS_API Option<size_t> getAssetIndex(String asset_id) const noexcept;
  ATLAS_API LinAlg::EigenConstRowView<double>
  getAssetSlice(size_t asset_index) const noexcept;
};

} // namespace Atlas