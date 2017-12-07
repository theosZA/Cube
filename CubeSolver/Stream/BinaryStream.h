#pragma once

#include <algorithm>
#include <iterator>
#include <istream>
#include <ostream>

namespace BinaryStream {

template <class T>
void Write(std::ostream& out, const T& value)
{
  auto buffer = reinterpret_cast<const char*>(&value);
  std::copy(buffer, buffer + sizeof(value), std::ostream_iterator<char>(out));
}

template <class T>
T Read(std::istream& in)
{
  T value{};
  auto buffer = reinterpret_cast<char*>(&value);
  in.read(buffer, sizeof(value));
  return value;
}

}