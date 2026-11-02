#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

void        printHelp(const char *progName);
bool        checkExtension(const std::string &fullString, const std::string &ending);
std::string findDefaultTexture(const std::string &objPath);

#endif 