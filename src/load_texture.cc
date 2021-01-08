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

#include "tilemancer/load_texture.h"

#include "tilemancer/image.h"

#include <iostream>

GLuint loadTexture(const std::string& path) {
  GLuint tex;
  ImageLoadResult ls = LoadImage(path, AlphaLoad::Include);
  if (ls.height <= 0 ){
    std::cout << "Failed to load image " + path << ": " << ls.error << std::endl;
    return 0;
  }

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  const int mode = ls.has_alpha ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, mode, ls.width, ls.height, 0, mode,
               GL_UNSIGNED_BYTE, &ls.components[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return tex;
}
