#include "Uri.hpp"
#include "Utils.hpp"
#include <cstddef>
#include <string>

void Uri::parse()
{
  // ex: "/another/path?param=value&param=value"
  std::string::size_type query_start = m_rawUri.find('?');

  if (query_start == std::string::npos)
  {
    m_path = m_rawUri;
    m_params.clear();
  }
  else
  {
    m_path = m_rawUri.substr(0, query_start);
    const std::string query_params_str = m_rawUri.substr(query_start + 1);

    m_params.clear();
    Uri::parseQueryParams(query_params_str);
  }
}

void Uri::parseQueryParams(const std::string& current_query_part_str)
{  
  // ex: "param=value&param=value"
  if (current_query_part_str.empty()) return;
  std::size_t separator_pos = current_query_part_str.find('='); 
  std::size_t delimiter_pos = current_query_part_str.find('&');
  std::string param_name;
  std::string param_value;

  if (separator_pos == std::string::npos)
  {
    param_name = url_decode(current_query_part_str.substr(0, delimiter_pos));
    param_value = "";
  }
  else
  {
    param_name = url_decode(current_query_part_str.substr(0, separator_pos));
    if (delimiter_pos == std::string::npos)
    {
      param_value = url_decode(current_query_part_str.substr(separator_pos + 1));
    }
    else
    {
      param_value = url_decode(current_query_part_str.substr(separator_pos + 1, delimiter_pos - (separator_pos  + 1)));
    }
  }
 
  m_params[param_name] = param_value;

  if (delimiter_pos != std::string::npos)
  {
    const std::string remaining_query_str = current_query_part_str.substr(delimiter_pos + 1);
    Uri::parseQueryParams(remaining_query_str);
  }
}
