/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <fstream>
#include <string>
#include <filesystem>

#include "fileManager.hpp"

namespace fs = std::filesystem;


void makeDirectory(std::string name)
{
    if (fs::is_directory(name) && fs::exists(name)) return;

    fs::create_directory(name);
}

bool fileExists(std::string path)
{
    return fs::exists(path);
}

void readFile(char* ptr, int len, std::string path)
{
    std::ifstream file(path, std::ios::binary);
    file.read(ptr, len);
    file.close();
}

void writeFile(char* ptr, int len, std::string name, std::string dirname)
{
    makeDirectory(dirname);

    std::ofstream file(dirname + "/" + name, std::ios::binary);
    file.write(ptr, len);
    file.close();
}