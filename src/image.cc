/*

  Tilemancer
  Copyright (C) 2016  Lucca Pedersoli Junior

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "tilemancer/image.h"

#include <cassert>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "stb_image_write.h"

unsigned char Select(int ch, unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
  switch(ch) {
    case 1: return a;
    case 2: return b;
    case 3: return c;
    case 4: return d;
    default:
      assert(false && "unhandled Select channel");
      return 0;
  }
}

ImageLoadResult LoadImage(const std::string& path, AlphaLoad alpha) {
  ImageLoadResult result;

  int channels = 0;
  unsigned char* data = stbi_load(path.c_str(), &result.width, &result.height, &channels, 0);

  if( data == NULL ) {
    result.error = stbi_failure_reason();
    result.components.resize(0);
    result.width = result.height = 0;
    result.has_alpha = false;
    return result;
  }

  result.has_alpha = false;
  if( alpha == AlphaLoad::Include ) {
    result.has_alpha = channels == 2 || channels == 4;
  }

  std::cout << "Image: " << path << " " << result.width << "x" << result.height << " alpha " << result.has_alpha << ".\n";

  const unsigned char dest_c = result.has_alpha ? 4 : 3;
  result.components.resize(result.width * result.height * dest_c, 0);

  for(int y=0; y<result.height; ++y) {
    for(int x=0; x<result.width; ++x) {
      const unsigned long src = (y * result.width + x) * channels;

      const unsigned char c1 = data[src + 0];
      const unsigned char c2 = (channels <= 1 ? 0 : data[src + 1]);
      const unsigned char c3 = (channels <= 2 ? 0 : data[src + 2]);
      const unsigned char c4 = (channels <= 3 ? 0 : data[src + 3]);

      const unsigned char r = c1;
      const unsigned char g = Select(channels, c1, c1, c2, c2);
      const unsigned char b = Select(channels, c1, c1, c3, c3);
      const unsigned char a = Select(channels, 255, c2, 255, c4);

      const unsigned long dest = (y * result.width + x) * dest_c;
      result.components[dest + 0] = r;
      result.components[dest + 1] = g;
      result.components[dest + 2] = b;

      if( result.has_alpha ) {
        result.components[dest + 3] = a;
      }
    }
  }

  stbi_image_free(data);
  return result;
}

bool SaveImage(const std::string& path, const std::vector<unsigned char>& pixels, int width, int height, AlphaSave has_alpha) {
  if (path.length() < 4) {
    return false;
  }

  const int channels = has_alpha == AlphaSave::Has ? 4 : 3;
  std::string ext = path.substr(path.length() - 4, 4);

  if (ext == ".png") {
     return stbi_write_png(path.c_str(), width, height, channels, pixels.data(), width * channels) != 0;
  }

  if (ext == ".bmp") {
     return stbi_write_bmp(path.c_str(), width, height, channels, pixels.data()) != 0;
  }

  if (ext == ".tga") {
    return stbi_write_tga(path.c_str(), width, height, channels, pixels.data()) != 0;
  }

  return false;
}
