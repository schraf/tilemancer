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

#include "tilemancer/socket.h"

#include "tilemancer/bezier.h"
#include "tilemancer/globals.h"
#include "tilemancer/image.h"

#include "SDL.h"

Socket::Socket() {
  futureN = -1;
  futureS = -1;
  index = 0;
  infloop = false;
  s = NULL;
  b = new Bezier();
  b->create();
}

Socket::~Socket() { delete b; }

void exportTexSingle(const std::string& dir) {
  const unsigned long size = texSizeX * texSizeY * 3;
  std::vector<unsigned char> pixels(size, 0);
  glBindTexture(GL_TEXTURE_2D, currentSocket->texture);

  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);

/*
  std::vector<GLfloat> fpixels(size, 0.0f);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &fpixels[0]);
  for (unsigned long i = 0; i < size; i++) {
    // scale?
    int val = fpixels[i];
    if (val > 255) {
      val = 255;
    } else if (val < 0) {
      val = 0;
    }
    pixels[0] = (Uint8)val;
  }
*/

  SaveImage(dir, pixels, texSizeX, texSizeY, AlphaSave::HasNot);
}
