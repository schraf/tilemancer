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

#include "tilemancer/drag.h"
#include "tilemancer/effect.h"
#include "tilemancer/gl.h"
#include "tilemancer/globals.h"
#include "tilemancer/graphics_globals.h"
#include "tilemancer/palette.h"
#include "tilemancer/parameter.h"
#include "tilemancer/texture.h"

void updateDrag(int layer, Effect* fx, Parameter* param) {
  if (layer != -1) {
    Texture* t = texs.at(currentTexture);
    fx->undone = true;
    t->genTex();
  } else if (layer == -1) {
    if (param->name == "Size") {
      texSizeX = param->value;
      texSizeY = param->value;
      glGenTextures(1, &screenTexFinal2);
      glBindTexture(GL_TEXTURE_2D, screenTexFinal2);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSizeX, texSizeY, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glGenFramebuffers(1, &screenFboFinal2);
      glBindFramebuffer(GL_FRAMEBUFFER, screenFboFinal2);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_2D, screenTexFinal2, 0);
      GLenum d2[] = {GL_COLOR_ATTACHMENT0};
      glDrawBuffers(1, d2);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      for (int i = 0; i < texs.size(); i++) {
        Texture* t = texs.at(i);
        for (int in = 0; in < t->fxs.size(); in++) {
          t->fxs.at(in)->undone = true;
        }
        t->genTex();
      }
    }
    if (param->name == "R" || param->name == "G" || param->name == "B") {
      Color rgb = Color(colorParams.at(1)->value, colorParams.at(2)->value,
                        colorParams.at(3)->value);
      Color hsv = RGBtoHSV(rgb.r, rgb.g, rgb.b);
      beforePaletteChange();
      colorParams.at(0)->value = hsv.r;
      colorParams.at(0)->value2 = hsv.g / 100.0;
      colorParams.at(0)->value3 = hsv.b / 100.0;
      colorParams.at(4)->value = hsv.r;
      colorParams.at(5)->value = hsv.g;
      colorParams.at(6)->value = hsv.b;
      if (selectedColor == -1) {
        rampA.r = rgb.r;
        rampA.g = rgb.g;
        rampA.b = rgb.b;
      } else if (selectedColor == -2) {
        rampB.r = rgb.r;
        rampB.g = rgb.g;
        rampB.b = rgb.b;
      } else {
        palette.at(selectedColor)->r = rgb.r;
        palette.at(selectedColor)->g = rgb.g;
        palette.at(selectedColor)->b = rgb.b;
      }
      paletteChanged();
    }
    if (param->name == "H" || param->name == "S" || param->name == "V") {
      int h = colorParams.at(4)->value;
      if (h == 360) {
        h = 0;
      }
      Color hsv = Color(h, colorParams.at(5)->value, colorParams.at(6)->value);
      Color rgb = HSVtoRGB(hsv.r, hsv.g, hsv.b);
      beforePaletteChange();
      colorParams.at(0)->value = hsv.r;
      colorParams.at(0)->value2 = hsv.g / 100.0;
      colorParams.at(0)->value3 = hsv.b / 100.0;
      colorParams.at(1)->value = rgb.r;
      colorParams.at(2)->value = rgb.g;
      colorParams.at(3)->value = rgb.b;
      if (selectedColor == -1) {
        rampA.r = rgb.r;
        rampA.g = rgb.g;
        rampA.b = rgb.b;
      } else if (selectedColor == -2) {
        rampB.r = rgb.r;
        rampB.g = rgb.g;
        rampB.b = rgb.b;
      } else {
        palette.at(selectedColor)->r = rgb.r;
        palette.at(selectedColor)->g = rgb.g;
        palette.at(selectedColor)->b = rgb.b;
      }
      paletteChanged();
    }
  }
}
