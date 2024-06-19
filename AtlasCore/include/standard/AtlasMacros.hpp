#pragma once

#define BEGIN_ATLAS_NAMESPACE namespace Atlas {
#define END_ATLAS_NAMESPACE }

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a##b
#define Err(msg) Err<AtlasException>(msg)

#define NAN_DOUBLE std::numeric_limits<double>::quiet_NaN()

#define EXPECT_TRUE(val, expr)                                                 \
  auto CONCAT(val, _opt) = expr;                                               \
  if (!CONCAT(val, _opt)) {                                                    \
    return Err<Atlas::AtlasException>(CONCAT(val, _opt).error());              \
  }

#define SAFE_MAP_INSERT(map, key, value)                                       \
  do {                                                                         \
    try {                                                                      \
      (map)[key] = value;                                                      \
    } catch (...) {                                                            \
      return Err<AtlasException>("MAP ALLOC FAILURE");                         \
    }                                                                          \
  } while (false)

#define EXPECT_FALSE(expr, msg)                                                \
  do {                                                                         \
    if (expr) {                                                                \
      return Err<AtlasException>(msg);                                         \
    }                                                                          \
  } while (false)