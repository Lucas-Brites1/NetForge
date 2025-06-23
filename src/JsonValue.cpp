#include "JsonValue.hpp"
#include <strings.h>
#include <cstddef>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include "Types.hpp"
#include "Utils.hpp"

bool JsonValue::asBoolean() const { return std::get<bool>(m_value); }
float64 JsonValue::asFloat() const { return std::get<double>(m_value); }
int JsonValue::asInteger() const { return std::get<int>(m_value); }
const std::string& JsonValue::asString() const { return std::get<std::string>(m_value); }
const JsonArray& JsonValue::asArray() const { return std::get<JsonArray>(m_value); }
const JsonObject& JsonValue::asObject() const { return std::get<JsonObject>(m_value); }

JsonValue& JsonValue::operator[](const std::string& key)
{
  JsonObject& json_object = std::get<JsonObject>(m_value);
  const auto& it = json_object.find(key);

  if (it == json_object.end()) { throw std::out_of_range("Key '" + key + "' not found."); };
  return it->second;
}

const JsonValue& JsonValue::operator[](const std::string& key) const
{
  const JsonObject& json_object = std::get<JsonObject>(m_value);
  const auto& it = json_object.find(key);

  if (it == json_object.end()) { throw std::out_of_range("Key '" + key + "' not found."); };
  return it->second;
}

JsonValue& JsonValue::operator[](std::size_t index)
{
  JsonArray& json_array = std::get<JsonArray>(m_value);
  
  if (index >= json_array.size()) { throw std::out_of_range("Index " + std::to_string(index) + " out of bound."); }
  return json_array[index];
}

const JsonValue& JsonValue::operator[](std::size_t index) const
{
  const JsonArray& json_array = std::get<JsonArray>(m_value);

  if (index >= json_array.size()) { throw std::out_of_range("Index " + std::to_string(index) + " out of bound."); }
  return json_array[index];
}

const std::string JsonValue::getIndent(int level, char indent_char, int indent_size) const
        { return std::string(level * indent_size, indent_char); }

const std::string JsonValue::toString(int indent_level) const
{
  std::string current_indent = getIndent(indent_level, m_dump_indent_char, m_dump_indent_size);
  std::string next_ident = getIndent(indent_level+1, m_dump_indent_char, m_dump_indent_size);

  switch (m_type)
  {
    case JsonType::NULLISH:
      {
      return "null";
      }
    case JsonType::BOOLEAN:
      {
        return std::get<bool>(m_value) ? "true" : "false";
      }
    case JsonType::INTEGER:
      {
        return std::to_string(std::get<int>(m_value));
      }
    case JsonType::FLOAT:
      {
        std::ostringstream oss;
        oss << std::setprecision(std::numeric_limits<float64>::max_digits10) << std::get<float64>(m_value);
        return oss.str();
      }
    case JsonType::STRING:
      {
        return escape_string(std::get<std::string>(m_value)); 
      }
    case JsonType::OBJECT:
      {
        std::string object_stringfied;
        const JsonObject& obj = std::get<JsonObject>(m_value);
        if (obj.empty()) { return "{}"; } 

        object_stringfied += "{";
        if (m_dump_indent_size > 0) object_stringfied += '\n';

        bool first_pair = true;
        for (const auto& pair : obj) 
        {
          if (!first_pair) {
            object_stringfied += ',';
            if (m_dump_indent_size > 0) object_stringfied += '\n';
          }
 
          if (m_dump_indent_size > 0) object_stringfied += next_ident;
          object_stringfied += escape_string(pair.first);
          object_stringfied += ":";
          if (m_dump_indent_size > 0) object_stringfied += m_dump_indent_char;
          
          object_stringfied += pair.second.toString(indent_level+1); 
          first_pair = false;
        }

        if (m_dump_indent_size > 0) 
        {
          object_stringfied += '\n';
          object_stringfied += current_indent;
        };

        object_stringfied += "}";
        return object_stringfied;
      }
    case JsonType::ARRAY:
      {
          return nullptr;
      }
  }
  return nullptr;
}

template<typename T>
T JsonValue::get() const { return m_value; };
