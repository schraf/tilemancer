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

#include "tilemancer/palette.h"
#include <SDL_image.h>
#include <SDL_surface.h>
#include <cmath>
#include <cstdlib>
#include "tilemancer/color.h"
#include "tilemancer/cpoint.h"
#include "tilemancer/effect.h"
#include "tilemancer/gl.h"
#include "tilemancer/globals.h"
#include "tilemancer/graphics_globals.h"
#include "tilemancer/parameter.h"
#include "tilemancer/texture.h"
#include "tilemancer/image.h"

Color* getPalColor(float H, float S, float V) {
  float C = V * S;
  float H2 = H / 60.0;
  float X = C * (1.0 - abs(fmod(H2, 2) - 1.0));
  float R, G, B;
  if (0 <= H2 && H2 < 1) {
    R = C;
    G = X;
    B = 0;
  } else if (1 <= H2 && H2 < 2) {
    R = X;
    G = C;
    B = 0;
  } else if (2 <= H2 && H2 < 3) {
    R = 0;
    G = C;
    B = X;
  } else if (3 <= H2 && H2 < 4) {
    R = 0;
    G = X;
    B = C;
  } else if (4 <= H2 && H2 < 5) {
    R = X;
    G = 0;
    B = C;
  } else if (5 <= H2 && H2 < 6) {
    R = C;
    G = 0;
    B = X;
  }
  float m = V - C;
  R += m;
  G += m;
  B += m;
  R *= 255;
  G *= 255;
  B *= 255;
  float dist = 2550.0;
  Color* final = NULL;
  for (int i = 0; i < palette.size(); i++) {
    Color* pal = palette.at(i);
    float dx = pal->r - R;
    float dy = pal->g - G;
    float dz = pal->b - B;
    float l = sqrt(dx * dx + dy * dy + dz * dz);
    if (l < dist && !pal->disabled) {
      dist = l;
      final = pal;
    }
  }
  return final;
}

void paletteChanged() {
  std::vector<GLubyte> byteData;
  for (int p = 0; p < 16 * 16; p++) {
    Color* color = palette.at(0);
    if (p < palette.size()) {
      color = palette.at(p);
    }
    byteData.push_back(GLubyte(color->r));
    byteData.push_back(GLubyte(color->g));
    byteData.push_back(GLubyte(color->b));
  }
  glGenTextures(1, &palImgReal);
  glBindTexture(GL_TEXTURE_2D, palImgReal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE,
               &byteData[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  bool indexed = false;
  if (colorParams.size() > 0) {
    indexed = colorParams.at(7)->value;
  }
  for (int i = 0; i < texs.size(); i++) {
    Texture* t = texs.at(i);
    bool updateTex = false;
    for (int e = 0; e < t->fxs.size(); e++) {
      Effect* fx = t->fxs.at(e);
      for (int p = 0; p < fx->params.size(); p++) {
        Parameter* param = fx->params.at(p);
        if (param->ID == 0) {
          if (indexed) {
            Color* color = palette.at(param->value4);
            Color hsv = RGBtoHSV(color->r, color->g, color->b);
            param->value = hsv.r;
            param->value2 = hsv.g / 100.0;
            param->value3 = hsv.b / 100.0;
          } else {
            float H = param->value;
            float S = param->value2;
            float V = param->value3;
            Color* final = getPalColor(H, S, V);
            for (int col = 0; col < palette.size(); col++) {
              Color* color = palette.at(col);
              if (* final == *color) {
                param->value4 = col;
              }
            }
          }
          fx->undone = true;
          updateTex = true;
        }
        if (param->ID == 7) {
          if (indexed) {
            for (int cp = 0; cp < param->points.size(); cp++) {
              Color* color = palette.at(param->points.at(cp)->i);
              Color hsv = RGBtoHSV(color->r, color->g, color->b);
              param->points.at(cp)->r = hsv.r;
              param->points.at(cp)->g = hsv.g / 100.0;
              param->points.at(cp)->b = hsv.b / 100.0;
            }
          } else {
            for (int cp = 0; cp < param->points.size(); cp++) {
              float H = param->points.at(cp)->r;
              float S = param->points.at(cp)->g;
              float V = param->points.at(cp)->b;
              Color* final = getPalColor(H, S, V);
              for (int col = 0; col < palette.size(); col++) {
                Color* color = palette.at(col);
                if (* final == *color) {
                  param->points.at(cp)->i = col;
                }
              }
            }
          }
          updateGrad(param);
          fx->undone = true;
          updateTex = true;
        }
      }
    }
    if (updateTex) {
      t->genTex();
    }
  }
}

void loadPalette() {
  for (int toDel = 0; toDel < palette.size(); toDel++) {
    delete palette.at(toDel);
  }
  palette.clear();
  glBindTexture(GL_TEXTURE_2D, palImg);
  int w, h;
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
  GLubyte* pixels = new GLubyte[w * h * 3];
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  if (w > 256 || h > 256) {
    if (errorMessage.size() > 0) {
      errorMessage += "\n";
    }
    errorMessage += "Image is too large";
    errorTimer = 160;

    palette.push_back(new Color(0, 0, 0));
    paletteChanged();
    selectedColor = 0;
  } else {
    for (int i = 0; i < w * h; i++) {
      Color* color =
          new Color(pixels[i * 3 + 0], pixels[i * 3 + 1], pixels[i * 3 + 2]);
      bool alreadyIn = false;
      for (int c2 = 0; c2 < palette.size(); c2++) {
        Color* color2 = palette.at(c2);
        if (*color == *color2) {
          alreadyIn = true;
        }
      }
      if (!alreadyIn) {
        palette.push_back(color);
      } else {
        delete color;
      }
    }
    delete[] pixels;
    selectedColor = 0;
    Color* rgb = palette.at(0);
    Color hsv = RGBtoHSV(rgb->r, rgb->g, rgb->b);
    colorParams.at(0)->value = hsv.r;
    colorParams.at(0)->value2 = hsv.g / 100.0;
    colorParams.at(0)->value3 = hsv.b / 100.0;
    colorParams.at(1)->value = rgb->r;
    colorParams.at(2)->value = rgb->g;
    colorParams.at(3)->value = rgb->b;
    colorParams.at(4)->value = hsv.r;
    colorParams.at(5)->value = hsv.g;
    colorParams.at(6)->value = hsv.b;
    paletteChanged();
  }
}

void exportPalette(const std::string& dir) {
  const int WIDTH = 16;
  const int HEIGHT = 16;
  std::vector<unsigned char> pixels(WIDTH * HEIGHT * 3, 0);
  glBindTexture(GL_TEXTURE_2D, palImgReal);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
  SaveImage(dir, pixels, WIDTH, HEIGHT, AlphaSave::HasNot);
}
