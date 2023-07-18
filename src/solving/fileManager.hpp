/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once


void makeDirectory(std::string name);

bool fileExists(std::string path);

void readFile(char* ptr, int len, std::string path);

void writeFile(char* ptr, int len, std::string name, std::string dirname);