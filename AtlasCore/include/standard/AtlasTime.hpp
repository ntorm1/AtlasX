#pragma once
#include "standard/AtlasTypes.hpp"
namespace Atlas {

//============================================================================
enum class TimeUnit {
  DAYS = 0,
  WEEKS = 1,
  MONTHS = 2,
};

//============================================================================
struct TimeOffset {
  TimeUnit type;
  size_t count;
};

Int64 applyTimeOffset(Int64 t, TimeOffset o);
int getMonthFromEpoch(Int64 epoch) noexcept;
Result<const char *> deduceDatetimeFormat(String const& datetime) noexcept;
Result<Int64> strToEpoch(const String &str, const String &dt_format) noexcept;
ATLAS_API String convertNanosecondsToTime(Int64 nanoseconds);

} // namespace Atlas