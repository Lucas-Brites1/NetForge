#include "JSON.hpp"
#include <initializer_list>
#include <utility>
#include "JsonValue.hpp"

template <typename T>
JSON::JSON(std::initializer_list<std::pair<const char*, T>> json_pair)
{
  m_JSONBuffer = Buffer();
  for (const auto& p : json_pair)
  {
    m_values[p.first] = JsonValue(p.second);
  }
};

JSON::JSON(std::initializer_list<std::pair<const char*, JsonValue>> json_pair)
{
  m_JSONBuffer = Buffer();
  for (const auto& p : json_pair)
  {
    m_values[p.first] = p.second;
  }
};

void JSON::serialize()
{
  // fix
  for (const auto& p : m_values)
  {
    std::string data_to_append = p.second.toString(m_indent);
    const char* data = data_to_append.c_str();
    std::size_t data_len = data_to_append.length();
    m_JSONBuffer.append(data, data_len);
  }
}

const char* JSON::Data() const { return m_JSONBuffer.data(); }

void JSON::Dump(int indent_level)
{
  if (indent_level > 10) return;

  m_indent = indent_level;
}
