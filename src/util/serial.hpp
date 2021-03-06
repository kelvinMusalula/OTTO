#pragma once

#include <gsl/span>
#include <string>
#include <tl/expected.hpp>

#include "util/exception.hpp"

namespace otto::util {

  using ConstBytesView = gsl::span<const std::uint8_t>;
  using BytesView = gsl::span<std::uint8_t>;

  /// Serial port communication
  struct Serial {

    /// @param vtime Read timeout in deciseconds
    /// @param vmin Minimum number of bytes to read before returning
    Serial(std::string path, int vtime = 0, int vmin = 1);

    ~Serial() noexcept;

    enum struct ErrorCode { empty_buffer, error };
    using exception = util::as_exception<ErrorCode>;

    tl::expected<void, exception> write(ConstBytesView);
    tl::expected<void, exception> read(BytesView dest) noexcept;
    tl::expected<std::vector<std::uint8_t>, exception> read(std::size_t n) noexcept;
    template<std::size_t N>
    tl::expected<std::array<std::uint8_t, N>, exception> read() noexcept;
    tl::expected<std::uint8_t, exception> read_single() noexcept;
    tl::expected<std::vector<std::uint8_t>, exception> read_line(
      std::uint8_t delim = '\n') noexcept;

    const std::string& path() noexcept
    {
      return path_;
    }

    int file_descriptor() noexcept
    {
      return fd_;
    }

  private:
    std::string path_;
    int fd_;
  };

  template<std::size_t N>
  tl::expected<std::array<std::uint8_t, N>, Serial::exception> Serial::read() noexcept
  {
    std::array<std::uint8_t, N> res;
    return read({res.data(), res.size()}).map([&] { return res; });
  }
} // namespace otto::util
