#ifndef BMP_LOADER_HPP
#define BMP_LOADER_HPP

#include <vector>

bool loadBMP(const char* filepath, std::vector<unsigned char>& pixels, int& width, int& height);

#endif