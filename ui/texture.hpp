/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <cstdint>

class Texture
{
public:
  Texture() = default;
  Texture(const std::string &path);
  ~Texture();

  void bind() const;
  void unbind() const;

private:
  unsigned int id;
};

#pragma pack(push, 1)

struct BMPheader
{
  // offset 0x00 = 0
  uint16_t type = 0x424D;
  uint32_t hSize;
  uint32_t reserved;
  uint32_t dataOffset;

  // offset 0x0E = 14
  uint32_t size = 0;
  int32_t width = 0;
  int32_t height = 0;

  // offset 0x1a = 26
  uint16_t planes = 1;
  uint16_t BPP = 0;
  uint32_t compression = 0;
  uint32_t imageSize = 0;
  int32_t xRes = 0;
  int32_t yRes = 0;
  uint32_t colorsUsed = 0;
  uint32_t importantColors = 0;
};

#pragma pack(pop)