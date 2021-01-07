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

#include "tilemancer/floatimage.h"
#include "tilemancer/color.h"
#include "tilemancer/globals.h"

void FloatImage::setAll(float v) {
  data.clear();
  for (int x = 0; x < texSizeX; x++) {
    for (int y = 0; y < texSizeY; y++) {
      for (int c = 0; c < 4; c++) {
        data.push_back(v);
      }
    }
  }
}

const float* FloatImage::ptr() const { return &data[0]; }

const std::vector<GLubyte> FloatImage::toByteArray() const {
  std::vector<GLubyte> bv;
  bv.reserve(data.size());

  for (int db = 0; db < data.size(); db++) {
    GLubyte c = fmax(0.0, fmin(255.0, data.at(db)));
    bv.push_back(c);
  }

  return bv;
}

void FloatImage::setColor(int x, int y, const Color& color, bool wrap) {
  bool render = true;
  if (x >= texSizeX) {
    if (wrap) {
      while (x >= texSizeX) {
        x -= texSizeX;
      }
    } else {
      render = false;
    }
  } else if (x < 0) {
    if (wrap) {
      while (x < 0) {
        x += texSizeX;
      }
    } else {
      render = false;
    }
  }
  if (y >= texSizeY) {
    if (wrap) {
      while (y >= texSizeY) {
        y -= texSizeY;
      }
    } else {
      render = false;
    }
  } else if (y < 0) {
    if (wrap) {
      while (y < 0) {
        y += texSizeY;
      }
    } else {
      render = false;
    }
  }
  if (render && data.size() > (y * texSizeX + x) * 4 + 3) {
    data[(y * texSizeX + x) * 4 + 0] = color.r;
    data[(y * texSizeX + x) * 4 + 1] = color.g;
    data[(y * texSizeX + x) * 4 + 2] = color.b;
    data[(y * texSizeX + x) * 4 + 3] = color.a;
  }
}

Color FloatImage::getColor(int x, int y, bool wrap) const {
  Color c = Color(0, 0, 0);
  bool render = true;
  if (x >= texSizeX) {
    if (wrap) {
      while (x >= texSizeX) {
        x -= texSizeX;
      }
    } else {
      render = false;
    }
  } else if (x < 0) {
    if (wrap) {
      while (x < 0) {
        x += texSizeX;
      }
    } else {
      render = false;
    }
  }
  if (y >= texSizeY) {
    if (wrap) {
      while (y >= texSizeY) {
        y -= texSizeY;
      }
    } else {
      render = false;
    }
  } else if (y < 0) {
    if (wrap) {
      while (y < 0) {
        y += texSizeY;
      }
    } else {
      render = false;
    }
  }
  if (render && data.size() > (y * texSizeX + x) * 4 + 3) {
    c.r = data[(y * texSizeX + x) * 4 + 0];
    c.g = data[(y * texSizeX + x) * 4 + 1];
    c.b = data[(y * texSizeX + x) * 4 + 2];
    c.a = data[(y * texSizeX + x) * 4 + 3];
  }
  return c;
}