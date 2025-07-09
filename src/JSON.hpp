#ifndef JSON_H
#define JSON_H
#include "JsonValue.hpp"
#include "Buffer.hpp"
#include <initializer_list>
#include <map>
#include <memory>

class JSON
{
  private:
    std::map<std::string, JsonValue> m_values;
    Buffer m_JSONBuffer;
    int m_indent = 1;
    
  public:
    JSON() : m_JSONBuffer() {};
    JSON(std::initializer_list<std::pair<const char*, JsonValue>> json_pairs);
    template<typename T>
    JSON(std::initializer_list<std::pair<const char*, T>> json_pair);

    void Dump(int indent_level = 1);
    const char* Data() const;
    void serialize();
    
    template<typename T>
    void deserialize(T& struct_to_deserialize);
    template<typename T>
    std::shared_ptr<T> deserialize();
};

#endif // !
