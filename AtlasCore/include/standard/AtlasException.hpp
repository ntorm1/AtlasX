#pragma once
#pragma once

#include <exception>
#include <string>

namespace Atlas {

class AtlasException : public std::exception {
private:
  std::string m_message;

public:
  AtlasException(std::string message) noexcept
      : m_message(std::move(message)) {}
  ~AtlasException() noexcept override = default;

  const char *what() const noexcept override { return m_message.c_str(); }
};

} // namespace Atlas