#ifndef BUFFER_H
#define BUFFER_H
#include <cstddef>
#include <stdexcept>
#include <vector>

constexpr unsigned int BUFFER_INITIAL_CAPACITY = 2048;

struct Buffer
{
  private:
    std::vector<char> buffer_data;
    std::size_t buffer_count;

  public:
    explicit Buffer(std::size_t initial_capacity = BUFFER_INITIAL_CAPACITY) : buffer_data(initial_capacity), buffer_count(0)
    {
      if (initial_capacity == 0) 
      {
        throw std::invalid_argument("Buffer capacity cannot be zero.");
      }
    }

    std::size_t capacity() const;
    std::size_t size() const;
    std::size_t availableSpace() const;
    
    const char* data() const;
    void realloc(std::size_t new_capacity);

    void append(const char* data, std::size_t data_len);
    char* write_ptr();
    void advance_write_ptr(std::size_t bytes_written);
    void advance_read_ptr(std::size_t bytes_read);
    void clear();
};

#endif // !BUFFER_H
