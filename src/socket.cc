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

#include "dirent.h"

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
  SDL_Surface* surface =
      SDL_CreateRGBSurface(SDL_SWSURFACE, texSizeX, texSizeY, 32, 0x000000FF,
                           0x0000FF00, 0x00FF0000, 0xFF000000);
  glBindTexture(GL_TEXTURE_2D, currentSocket->texture);
  GLfloat* pixels = new GLfloat[int(texSizeX * texSizeY * 4)];
  if (OS & Windows) {
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
  } else if (OS & Unix) {
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);
    for (int i = 0; i < texSizeX * texSizeY * 4; i++) {
      Uint8* p =
          (Uint8*)surface->pixels + i * surface->format->BytesPerPixel / 4;
      int val = pixels[i];
      if (val > 255) {
        val = 255;
      } else if (val < 0) {
        val = 0;
      }
      p[0] = (Uint8)val;
    }
  }
  IMG_SavePNG(surface, dir.c_str());
  SDL_FreeSurface(surface);
}
