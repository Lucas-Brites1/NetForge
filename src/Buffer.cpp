#include "Buffer.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <stdexcept>

std::size_t Buffer::capacity()       const { return buffer_data.capacity(); }
std::size_t Buffer::size()           const { return buffer_count; }
std::size_t Buffer::availableSpace() const { return Buffer::capacity() - buffer_count; }

void Buffer::realloc(std::size_t new_capacity)
{
  if (new_capacity < capacity() || new_capacity == 0)
  {
    throw std::invalid_argument("Buffer new capacity must be greater than 0 or greater than current capacity.");
  }

  buffer_data.reserve(new_capacity);
}

const char* Buffer::data() const
{
  return buffer_data.data();
}

void Buffer::append(const char* data, std::size_t data_len)
{
  if (data_len == 0 || data == nullptr) return;

  std::size_t new_total_data_count = buffer_count + data_len;

  if (new_total_data_count > buffer_data.capacity())
  {
    std::size_t new_capacity = std::max(new_total_data_count, buffer_data.capacity() * 2);
    Buffer::realloc(new_capacity);
  }

  if (new_total_data_count > buffer_data.size())
  {
    buffer_data.resize(new_total_data_count);
  }

  std::memcpy(&buffer_data[buffer_count], data, data_len);
  buffer_count += data_len;
}

char* Buffer::write_ptr()
{
  return buffer_data.data() + buffer_count;
}

void Buffer::advance_write_ptr(std::size_t bytes_written)
{
  if (bytes_written == 0) return;
  
  std::size_t new_total_data_count = buffer_count + bytes_written;

  if (new_total_data_count > buffer_data.capacity())
  {
    std::size_t new_capacity = std::max(new_total_data_count, buffer_data.capacity() * 2);
    Buffer::realloc(new_capacity);
  }

  if (new_total_data_count > buffer_data.size())
  {
    buffer_data.resize(new_total_data_count);
  }

  buffer_count += bytes_written;
}

void Buffer::advance_read_ptr(std::size_t bytes_read)
{
  if (bytes_read == 0) return;

  if (bytes_read > buffer_count)
  {
    throw std::range_error("Attempted to read more bytes than available in buffer.");
  }

  if (bytes_read < buffer_count)
  {
    std::memmove(buffer_data.data(), buffer_data.data() + bytes_read, buffer_count - bytes_read);
  }

  buffer_count -= bytes_read;
}

void Buffer::clear()
{
  buffer_count = 0;
}

std::string Buffer::get_from_to(std::size_t start_idx, std::size_t end_idx)
{
  if (start_idx >= buffer_count || end_idx >= buffer_count || start_idx > end_idx)
  {
    throw std::out_of_range("Indexes range out of bound.");
  }

  std::size_t len = end_idx - start_idx + 1;
  return std::string(this->buffer_data.data() + start_idx, len);
}

std::size_t Buffer::find(char character_to_find, std::size_t start_position_to_find) 
{
  if (start_position_to_find >= buffer_count) { return std::string::npos; }

  std::size_t current_pos = start_position_to_find;
  char *current_ptr = this->buffer_data.data() + start_position_to_find;
  while(current_pos < buffer_count)
  {
    if (*current_ptr == character_to_find) return current_pos;
    current_pos++;
    current_ptr++;
  }

  return std::string::npos;
}

std::size_t Buffer::find(const std::string& pattern, std::size_t start_position_to_find)
{
    if (pattern.empty()) {
        return start_position_to_find;
    }
    if (start_position_to_find >= buffer_count) {
        return std::string::npos;
    }
    if (pattern.length() > (buffer_count - start_position_to_find)) {
        return std::string::npos;
    }

    for (std::size_t i = start_position_to_find; i <= buffer_count - pattern.length(); ++i) {
        bool match = true;
        for (std::size_t j = 0; j < pattern.length(); ++j) {
            if (this->buffer_data.data()[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            return i; 
        }
    }

    return std::string::npos;
}
