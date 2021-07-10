#include "File.h"

// static
std::vector<uint8_t> File::LoadInMemory(const char* filepath) 
{
  std::vector<uint8_t> buffer;

  FILE* file = fopen(filepath, "rb");
  if (!file) {
    return buffer;
  }

  // Get the filesize
  fseek(file, 0, SEEK_END);
  size_t filesize = ftell(file);
  fseek(file, 0, SEEK_SET);

  buffer.resize(filesize);
  size_t size_read = fread(buffer.data(), 1, filesize, file);
  fclose(file);

  if (size_read != filesize) {
    buffer.clear();
  }
  return buffer;
}