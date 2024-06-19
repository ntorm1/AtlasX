#include "pch.h"

#include "exchange/Asset.hpp"

#include "standard/AtlasTime.hpp"

BEGIN_ATLAS_NAMESPACE
//============================================================================
Result<bool> Asset::loadCSV() {
  assert(source);
  try {
    // Open the file
    std::ifstream file(*source);
    if (!file.is_open()) {
      return Err("Failed to open file: " + *source);
    }
    // get row count
    rows = 0;
    std::string line;
    while (std::getline(file, line)) {
      rows++;
    }
    rows--;
    file.clear();                 // Clear any error flags
    file.seekg(0, std::ios::beg); // Move the file pointer back to the start

    // Parse headers
    if (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string columnName;
      int columnIndex = 0;

      // Skip the first column (date)
      std::getline(ss, columnName, ',');
      while (std::getline(ss, columnName, ',')) {
        headers[columnName] = columnIndex;
        columnIndex++;
      }
    } else {
      return Err("Could not parse headers");
    }
    cols = headers.size();
    resize(rows, cols);

    size_t row_counter = 0;
    Option<String> datetime_format = std::nullopt;
    while (std::getline(file, line)) {
      std::stringstream ss(line);

      // First column is datetime
      std::string timestamp, columnValue;
      std::getline(ss, timestamp, ',');

      // try to convert string to epoch time
      int64_t epoch_time = 0;
      if (!datetime_format) {
        auto res  = deduceDatetimeFormat(timestamp);
        if (!res) {
          String msg = std::format("Could not deduce datetime format for: {}", timestamp);
          return Err(msg);
				}
        String format = res.value();
        datetime_format = format;
      }
      if (datetime_format != "") {
        auto res = strToEpoch(timestamp, *datetime_format);
        if (res && res.value() > 0) {
          epoch_time = res.value();
        }
      } else {
        try {
          epoch_time = std::stoll(timestamp);
        } catch (...) {
        }
      }
      if (epoch_time == 0) {
        return Err("Invalid timestamp: " + timestamp +
                   ", epoch time is: " + std::to_string(epoch_time));
      }
      timestamps[row_counter] = epoch_time;

      int col_idx = 0;
      while (std::getline(ss, columnValue, ',')) {
        double value = std::stod(columnValue);
        size_t index = row_counter * cols + col_idx;
        data[index] = value;
        col_idx++;
      }
      row_counter++;
    }
    return true;
  } catch (const std::exception &e) {
    return Err("Error loading CSV: " + std::string(e.what()));
  } catch (...) {
    return Err("Error loading CSV: Unknown error");
  }
}

END_ATLAS_NAMESPACE