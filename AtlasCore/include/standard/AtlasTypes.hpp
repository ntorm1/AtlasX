#pragma once
#ifdef ATLASCORE_EXPORTS
#define ATLAS_API __declspec(dllexport)
#else
#define ATLAS_API __declspec(dllimport)
#endif
#include "../../external/expected.hpp"
#include "AtlasException.hpp"
#include "AtlasMacros.hpp"
#include "AtlasDeclare.hpp"
#include <cstdint>
#include "pch.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace Atlas {
template <typename T> using UniquePtr = std::unique_ptr<T>;

template <typename T> using SharedPtr = std::shared_ptr<T>;

template <typename T> using Result = tl::expected<T, AtlasException>;

template <typename T> using Option = std::optional<T>;

template <typename K, typename V> using HashMap = std::unordered_map<K, V>;

template <typename K, typename V> using UnorderedMap = ankerl::unordered_dense::map<K, V>;

template <typename T> using Vector = std::vector<T>;

template <typename T> using Err = tl::unexpected<T>;

template <typename T> using Set = std::set<T>;

using String = std::string;
using StringRef = std::string_view;

using Int = int;
using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;

using Uint = unsigned int;
using Uint8 = uint8_t;
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint64 = uint64_t;

} // namespace Atlas