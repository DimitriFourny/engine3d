#pragma once
#include <vector>

class File {
public:
  static std::vector<uint8_t> LoadInMemory(const char* filepath);
};