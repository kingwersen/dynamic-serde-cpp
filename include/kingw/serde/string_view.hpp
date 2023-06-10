#pragma once

#include <cstddef>
#include <cstring>
#include <string>

// The following was taken from fmtlib, with some simplifications:
// https://github.com/fmtlib/fmt/blob/master/include/fmt/core.h


namespace kingw {
namespace serde {

/**
  An implementation of ``std::basic_string_view`` for pre-C++17. It provides a
  subset of the API. ``fmt::basic_string_view`` is used for format strings even
  if ``std::string_view`` is available to prevent issues when a library is
  compiled with a different ``-std`` option than the client code (which is not
  recommended).
 */
class string_view {
 private:
  const char* data_;
  size_t size_;

 public:
  using value_type = char;
  using iterator = const char*;

  constexpr string_view() noexcept : data_(nullptr), size_(0) {}

  /** Constructs a string reference object from a C string and a size. */
  constexpr string_view(const char* s, std::size_t count) noexcept
      : data_(s), size_(count) {}

  /**
    \rst
    Constructs a string reference object from a C string computing
    the size with ``std::char_traits<Char>::length``.
    \endrst
   */
  constexpr string_view(const char* s)
      : data_(s), size_(std::strlen(s)) {}

  /** Constructs a string reference from a ``std::basic_string`` object. */
  string_view(
      const std::string& s) noexcept
      : data_(s.data()), size_(s.size()) {}

  /** Returns a pointer to the string data. */
  constexpr auto data() const noexcept -> const char* { return data_; }

  /** Returns the string size. */
  constexpr auto size() const noexcept -> std::size_t { return size_; }

  constexpr auto begin() const noexcept -> iterator { return data_; }
  constexpr auto end() const noexcept -> iterator { return data_ + size_; }

  constexpr auto operator[](size_t pos) const noexcept -> const char& {
    return data_[pos];
  }

  constexpr void remove_prefix(size_t n) noexcept {
    data_ += n;
    size_ -= n;
  }

  constexpr bool starts_with(
      string_view sv) const noexcept {
    return size_ >= sv.size_ &&
           std::char_traits<char>::compare(data_, sv.data_, sv.size_) == 0;
  }
  constexpr bool starts_with(char c) const noexcept {
    return size_ >= 1 && std::char_traits<char>::eq(*data_, c);
  }
  constexpr bool starts_with(const char* s) const {
    return starts_with(string_view(s));
  }

  // Lexicographically compare this string reference to other.
  auto compare(string_view other) const -> int {
    size_t str_size = size_ < other.size_ ? size_ : other.size_;
    int result = std::char_traits<char>::compare(data_, other.data_, str_size);
    if (result == 0)
      result = size_ == other.size_ ? 0 : (size_ < other.size_ ? -1 : 1);
    return result;
  }

  friend auto operator==(string_view lhs,
                                                   string_view rhs)
      -> bool {
    return lhs.compare(rhs) == 0;
  }
  friend auto operator!=(string_view lhs, string_view rhs) -> bool {
    return lhs.compare(rhs) != 0;
  }
  friend auto operator<(string_view lhs, string_view rhs) -> bool {
    return lhs.compare(rhs) < 0;
  }
  friend auto operator<=(string_view lhs, string_view rhs) -> bool {
    return lhs.compare(rhs) <= 0;
  }
  friend auto operator>(string_view lhs, string_view rhs) -> bool {
    return lhs.compare(rhs) > 0;
  }
  friend auto operator>=(string_view lhs, string_view rhs) -> bool {
    return lhs.compare(rhs) >= 0;
  }
};

}  // namespace serde
}  // namespace kingw
