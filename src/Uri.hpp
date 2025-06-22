#ifndef URI_H
#define URI_H
#include <map>
#include <string>

struct Uri 
{
  public:
    Uri(const std::string& rawURI)
    {
      m_rawUri = rawURI;
      parse();
    }

    Uri& operator=(const std::string& rawUri) 
    { 
      m_rawUri = rawUri;
      parse();
      return *this;
    }

    const std::string& getPath() const { return this->m_path; };
    const std::map<std::string, std::string>& getQueryParams() const { return this->m_params; };
    const std::string getQueryParam(const std::string& key) const 
    { 
      auto it = m_params.find(key);
      if (it != m_params.end()) return it->second;
      return "";
    };
    const std::string& getRawUri() const { return this->m_rawUri; };

  private:
    std::string m_rawUri;
    std::string m_path;
    std::map<std::string, std::string> m_params;
    
    // ex: "/another/path?param=value"
    void parse();
    void parseQueryParams(const std::string& query_params_str);
};

#endif // URI_H!
