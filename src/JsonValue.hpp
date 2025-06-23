#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <cstddef>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include "Types.hpp"

class JsonValue;

using JsonArray = std::vector<JsonValue>;
using JsonObject = std::map<std::string, JsonValue>;

class JsonValue 
{
  
  public:
    enum JsonType
    {
      NULLISH,
      BOOLEAN,
      INTEGER,
      FLOAT,
      STRING,
      OBJECT,
      ARRAY,
    };

    int m_dump_indent_size = 0x0002;
    char m_dump_indent_char = ' ';

  private:
    const std::string getIndent(int level, char ident_char, int indent_size) const;
  
  private:
    JsonType m_type;
    std::variant<
      std::nullptr_t,
      bool,
      float64,
      int,
      std::string,
      JsonObject,
      JsonArray
      > m_value;

  public:
    JsonValue() : m_type(JsonValue::JsonType::NULLISH), m_value(nullptr) {};
    JsonValue(bool val) : m_type(JsonValue::JsonType::BOOLEAN), m_value(val) {};
    JsonValue(int val) : m_type(JsonValue::JsonType::INTEGER), m_value(val) {};
    JsonValue(float64 val) : m_type(JsonValue::JsonType::FLOAT), m_value(val) {};
    JsonValue(const char* val) : m_type(JsonValue::JsonType::STRING), m_value(static_cast<std::string>(val)) {};
    JsonValue(std::string val) : m_type(JsonValue::JsonType::STRING), m_value(val) {};
    JsonValue(std::initializer_list<JsonValue> init_list) : m_type(JsonValue::JsonType::ARRAY), m_value(init_list) {};
    JsonValue(JsonObject val) : m_type(JsonValue::JsonType::OBJECT), m_value(val) {};

    template<typename T>
    static JsonValue Object(std::initializer_list<std::pair<const char*, T>> list)
    {
      JsonObject obj;
      for (const auto& p : list)
      {
        obj[p.first] = JsonValue(p.second); 
      }
      return obj;
    }

    static JsonValue Object(std::initializer_list<std::pair<const char*, JsonValue>> list)
    {
      JsonObject obj;
      for (const auto& p : list)
      {
        obj[p.first] = p.second;
      }
      return obj;
    }

    JsonType get_type() { return this->m_type; };
    bool asBoolean() const;
    float64 asFloat() const;
    int asInteger() const;
    const std::string& asString() const;
    const JsonArray& asArray() const;
    const JsonObject& asObject() const;

    JsonValue& operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;

    JsonValue& operator[](std::size_t index);
    const JsonValue& operator[](std::size_t index) const;

    const std::string toString(int indent_level) const;
    template<typename T>
    T get() const;
};

#endif // !JSON_VALUE_H
