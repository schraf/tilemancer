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

#include "tilemancer/parameter.h"
#include "tilemancer/color.h"
#include "tilemancer/cpoint.h"
#include "tilemancer/drag.h"
#include "tilemancer/effect.h"
#include "tilemancer/gl.h"
#include "tilemancer/globals.h"
#include "tilemancer/math.h"
#include "tilemancer/palette.h"
#include "tilemancer/render.h"
#include "tilemancer/socket.h"
#include "tilemancer/texture.h"
#include "tilemancer/undoredo.h"
#include "tilemancer/graphics_globals.h"

Parameter::~Parameter() {}

void Parameter::render(int ex, int ey) {  // renderparam
  if (ID == 0) {
    renderWheel(ex + x, ey + y, w, h, 0);
    renderWheelTri(ex + x, ey + y, w, h, value);

    float centerX = x + w / 2.0;
    float centerY = y + h / 2.0;
    float len = w / 2.0 * 0.8;
    float len2 = w / 2.0 * 0.9;
    float x1 = cos((value)*M_PI / 180.0) * len + centerX;  // 1, 1
    float y1 = sin((value)*M_PI / 180.0) * len + centerY;
    float xf = cos((value)*M_PI / 180.0) * len2 + centerX;  // 1, 1
    float yf = sin((value)*M_PI / 180.0) * len2 + centerY;
    float x2 = cos((value + 120.0) * M_PI / 180.0) * len + centerX;  // 0, 1
    float y2 = sin((value + 120.0) * M_PI / 180.0) * len + centerY;
    float x3 = cos((value + 240.0) * M_PI / 180.0) * len + centerX;  // 0, 0
    float y3 = sin((value + 240.0) * M_PI / 180.0) * len + centerY;

    float v1x = x1 - x2;
    float v1y = y1 - y2;
    float l1 = sqrt(v1x * v1x + v1y * v1y);
    v1x /= l1;
    v1y /= l1;
    l1 *= value2 * value3;
    v1x *= l1;
    v1y *= l1;
    float v2x = x2 - x3;
    float v2y = y2 - y3;
    float l2 = sqrt(v2x * v2x + v2y * v2y);
    v2x /= l2;
    v2y /= l2;
    l2 *= value3;
    v2x *= l2;
    v2y *= l2;

    renderIcon(ex + x3 - 4 + v1x + v2x, ey + y3 - 4 + v1y + v2y, 8, 8, iconImg2,
               0);
    renderIcon(ex + xf - 4, ey + yf - 4, 8, 8, iconImg2, 0);
  } else if (ID == 4) {
    renderWheelF(ex + x, ey + y, w, h, 0);
    renderWheelTriF(ex + x, ey + y, w, h, value);

    float centerX = x + w / 2.0;
    float centerY = y + h / 2.0;
    float len = w / 2.0 * 0.8;
    float len2 = w / 2.0 * 0.9;
    float x1 = cos((value)*M_PI / 180.0) * len + centerX;  // 1, 1
    float y1 = sin((value)*M_PI / 180.0) * len + centerY;
    float xf = cos((value)*M_PI / 180.0) * len2 + centerX;  // 1, 1
    float yf = sin((value)*M_PI / 180.0) * len2 + centerY;
    float x2 = cos((value + 120.0) * M_PI / 180.0) * len + centerX;  // 0, 1
    float y2 = sin((value + 120.0) * M_PI / 180.0) * len + centerY;
    float x3 = cos((value + 240.0) * M_PI / 180.0) * len + centerX;  // 0, 0
    float y3 = sin((value + 240.0) * M_PI / 180.0) * len + centerY;

    float v1x = x1 - x2;
    float v1y = y1 - y2;
    float l1 = sqrt(v1x * v1x + v1y * v1y);
    v1x /= l1;
    v1y /= l1;
    l1 *= value2 * value3;
    v1x *= l1;
    v1y *= l1;
    float v2x = x2 - x3;
    float v2y = y2 - y3;
    float l2 = sqrt(v2x * v2x + v2y * v2y);
    v2x /= l2;
    v2y /= l2;
    l2 *= value3;
    v2x *= l2;
    v2y *= l2;

    renderIcon(ex + x3 - 4 + v1x + v2x, ey + y3 - 4 + v1y + v2y, 8, 8, iconImg2,
               0);
    renderIcon(ex + xf - 4, ey + yf - 4, 8, 8, iconImg2, 0);
  } else if (ID == 1) {
    renderUI(ex + x, ey + y, w, h, effectImg6);
    float space = h / 2 - 4;
    renderText(name, ex + x + space, ey + y + space, fontImg, 0);
    if (s == NULL || s->s == NULL) {
      if (textType == this) {
        if (blinkTimer <= 32) {
          renderText(typing + "_", ex + x + w - space, ey + y + space, fontImg,
                     true);
        } else {
          renderText(typing, ex + x + w - space - 5, ey + y + space, fontImg,
                     true);
        }
      } else {
        renderNumber(value, ex + x + w - space, ey + y + space, fontImg, false,
                     true);
      }
    }
  } else if (ID == 5) {  // unused
    renderUI(ex + x, ey + y, w, h, effectImg6);
    float space = h / 2 - 4;
    renderText(name, ex + x + space, ey + y + space, fontImg, 0);
    renderNumber(pow(2, value), ex + x + w - space, ey + y + space, fontImg,
                 false, true);
  } else if (ID == 2) {
    renderUI(ex + x, ey + y, w, h, effectImg6);
    float space = h / 2 - 4;
    renderText(name, ex + x + space, ey + y + space, fontImg, 0);
    if (s == NULL || s->s == NULL) {
      if (textType == this) {
        if (blinkTimer <= 32) {
          renderText(typing + "_", ex + x + w - space, ey + y + space, fontImg,
                     true);
        } else {
          renderText(typing, ex + x + w - space - 5, ey + y + space, fontImg,
                     true);
        }
      } else {
        renderNumber(value, ex + x + w - space, ey + y + space, fontImg, true,
                     true);
      }
    }
  } else if (ID == 3) {
    renderUI(ex + x, ey + y, w, h, effectImg6);
    float space = h / 2 - 4;
    renderText(name, ex + x + space, ey + y + space, fontImg, 0);
    if (value == 0) {
      renderText("NO", ex + x + w - space, ey + y + space, fontImg, true);
    } else {
      renderText("YES", ex + x + w - space, ey + y + space, fontImg, true);
    }
  } else if (ID == 7) {
    for (int i = 0; i < points.size(); i++) {
      float a1 = 0.0;
      for (int pp = 0; pp < points.size(); pp++) {
        if (points.at(pp)->a < points.at(i)->a && pp != i) {
          a1 = points.at(i)->a;
        }
      }
      float a2 = 1.0;
      for (int pp = 0; pp < points.size(); pp++) {
        if (points.at(pp)->a > points.at(i)->a && points.at(pp)->a < a2 &&
            pp != i) {
          a2 = points.at(pp)->a;
        }
      }
      Color* final =
          getPalColor(points.at(i)->r, points.at(i)->g, points.at(i)->b);
      renderColor(ex + x + 4 + int(a1 * 100), ey + y + 3,
                  int(a2 * 100) - int(a1 * 100), 4, final);
    }
    for (int i = 0; i < points.size(); i++) {
      renderIcon(ex + x + points.at(i)->a * 100, ey + y + 1, 8, 8, iconImg12,
                 (i == selectedPoint));
    }
  } else if (ID == 8) {
    renderIcon(ex + x, ey + y, 8, 8, iconImg13, 0);
  } else if (ID == 9) {
    renderIcon(ex + x, ey + y, 8, 8, iconImg13, 1);
  }
}

Parameter::Parameter(int ID, std::string name, float x, float y, float w,
                     float h, int value, int value2, int value3,
                     std::string tt) {
  this->index = 0;
  this->s = NULL;
  this->ID = ID;
  this->name = name;
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
  this->tt = tt;
  this->resetValue = value;
  this->value = value;
  this->value2 = value2;
  this->value3 = value3;
  this->value4 = 0;
  this->selectedPoint = 0;
  this->dragging = false;
  this->dragging2 = false;
  if (ID == 0) {
    float H = value;
    float S = value2;
    float V = value3;
    Color* final = getPalColor(H, S, V);
    for (int col = 0; col < palette.size(); col++) {
      Color* color = palette.at(col);
      if (* final == *color) {
        value4 = col;
      }
    }
  } else if (ID == 7) {
    CPoint* a = new CPoint();
    a->a = 0.0;
    a->r = 0;
    a->g = 0;
    a->b = 1;
    points.push_back(a);
  }
}

void Parameter::mouseDown(int mx, int my, int ex, int ey, int layer,
                          Effect* fx) {
  mx -= ex;
  my -= ey;
  if (ID == 0 || ID == 4) {
    float centerX = x + w / 2.0;
    float centerY = y + h / 2.0;
    float dx = mx - centerX;
    float dy = my - centerY;
    float dist = sqrt(dx * dx + dy * dy);
    if (dist < w / 2.0 && dist > w / 2.0 * 0.8) {
      saveUndo();
      dragging = true;
      value = atan2(-dy, -dx) / M_PI * 180.0 + 180.0;
      if (value == 360) {
        value = 0;
      }
      if (value < 0 || value > 360) {
        value = 0;
      }
    }
    float len = w / 2.0 * 0.8;
    float x1 = cos((value)*M_PI / 180.0) * len + centerX;  // 1, 1
    float y1 = sin((value)*M_PI / 180.0) * len + centerY;
    float x2 = cos((value + 120.0) * M_PI / 180.0) * len + centerX;  // 0, 1
    float y2 = sin((value + 120.0) * M_PI / 180.0) * len + centerY;
    float x3 = cos((value + 240.0) * M_PI / 180.0) * len + centerX;  // 0, 0
    float y3 = sin((value + 240.0) * M_PI / 180.0) * len + centerY;
    dragging2 = insideTri(mx, my, x1, y1, x2, y2, x3, y3);
    if (dragging2) {
      saveUndo();
      float oldX = mx - x3;
      float oldY = my - y3;
      float angle = 30.0 - value;
      float newX = (oldX * sin((120 - angle) * M_PI / 180.0) +
                    oldY * sin((120 - 90 - angle) * M_PI / 180.0)) /
                   sin(120 * M_PI / 180.0);
      float newY = (oldX * sin(angle * M_PI / 180.0) +
                    oldY * sin((90 + angle) * M_PI / 180.0)) /
                   sin(120 * M_PI / 180.0);

      float v1x = x1 - x2;
      float v1y = y1 - y2;
      float l1 = sqrt(v1x * v1x + v1y * v1y);
      float v2x = x2 - x3;
      float v2y = y2 - y3;
      float l2 = sqrt(v2x * v2x + v2y * v2y);

      newX /= l1;
      newY /= l2;

      value2 = newX / newY;
      value3 = newY;
      if (value2 < 0.0) {
        value2 = 0.0;
      } else if (value2 > 1.0) {
        value2 = 1.0;
      }
      if (value3 < 0.0) {
        value3 = 0.0;
      } else if (value3 > 1.0) {
        value3 = 1.0;
      }
      float H = value;
      float S = value2;
      float V = value3;
      Color* final = getPalColor(H, S, V);
      for (int col = 0; col < palette.size(); col++) {
        Color* color = palette.at(col);
        if (* final == *color) {
          value4 = col;
        }
      }
    }
    if ((dragging || dragging2) && layer == -1) {
      Color hsv = Color(value, value2 * 100, value3 * 100);
      Color rgb = HSVtoRGB(hsv.r, hsv.g, hsv.b);
      beforePaletteChange();
      colorParams.at(1)->value = rgb.r;
      colorParams.at(2)->value = rgb.g;
      colorParams.at(3)->value = rgb.b;
      colorParams.at(4)->value = value;
      colorParams.at(5)->value = value2 * 100;
      colorParams.at(6)->value = value3 * 100;
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
    if (ID == 0) {
      p->points.at(p->selectedPoint)->r = value;
      p->points.at(p->selectedPoint)->g = value2;
      p->points.at(p->selectedPoint)->b = value3;
      p->points.at(p->selectedPoint)->i = value4;
      updateGrad(p);
      if (layer != -1) {
        Texture* t = texs.at(currentTexture);
        fx->undone = true;
        t->genTex();
      }
    }
  } else if (ID == 7) {
    for (int i = 0; i < points.size(); i++) {
      if (mx > x + points.at(i)->a * 100 &&
          mx < x + points.at(i)->a * 100 + 8 && my > y + 1 && my < y + 1 + 8) {
        selectedPoint = i;
        p->value = points.at(selectedPoint)->r;
        p->value2 = points.at(selectedPoint)->g;
        p->value3 = points.at(selectedPoint)->b;
        p->value4 = points.at(selectedPoint)->i;
        dragging = true;
      }
    }
  } else if (ID == 8) {
    if (mx > x && mx < x + 8 && my > y && my < y + 8) {
      CPoint* a = new CPoint();
      float a1 = p->points.at(p->selectedPoint)->a;
      float a2 = 1.0;
      for (int pp = 0; pp < p->points.size(); pp++) {
        if (p->points.at(pp)->a > a1 && p->points.at(pp)->a < a2) {
          a2 = p->points.at(pp)->a;
        }
      }
      a->a = (a1 + a2) / 2.0;
      a->r = 0;
      a->g = 0;
      a->b = 1;
      p->points.push_back(a);
      p->selectedPoint = p->points.size() - 1;
      p->p->value = p->points.at(p->selectedPoint)->r;
      p->p->value2 = p->points.at(p->selectedPoint)->g;
      p->p->value3 = p->points.at(p->selectedPoint)->b;
      p->p->value4 = p->points.at(p->selectedPoint)->i;
      updateGrad(p);
      if (layer != -1) {
        Texture* t = texs.at(currentTexture);
        fx->undone = true;
        t->genTex();
      }
    }
  } else if (ID == 9) {
    if (mx > x && mx < x + 8 && my > y && my < y + 8) {
      if (p->points.size() > 1) {
        delete p->points.at(p->selectedPoint);
        p->points.erase(p->points.begin() + p->selectedPoint);
        if (p->selectedPoint >= p->points.size()) {
          p->selectedPoint = p->points.size() - 1;
          p->p->value = p->points.at(p->selectedPoint)->r;
          p->p->value2 = p->points.at(p->selectedPoint)->g;
          p->p->value3 = p->points.at(p->selectedPoint)->b;
          p->p->value4 = p->points.at(p->selectedPoint)->i;
        }
        updateGrad(p);
        if (layer != -1) {
          Texture* t = texs.at(currentTexture);
          fx->undone = true;
          t->genTex();
        }
      }
    }
  } else if (ID == 1 || ID == 2 || ID == 3 || ID == 5 || ID == 6) {
    if (mx > x && mx < x + w && my > y && my < y + h) {
      if (doubleClickTimer <= 20) {
        value = resetValue;
        if (layer == -1) {
          if (name == "Size") {
            texSizeX = value;
            texSizeY = value;
            glGenTextures(1, &screenTexFinal2);
            glBindTexture(GL_TEXTURE_2D, screenTexFinal2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSizeX, texSizeY, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, 0);
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
          if (name == "R" || name == "G" || name == "B") {
            Color rgb =
                Color(colorParams.at(1)->value, colorParams.at(2)->value,
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
          if (name == "H" || name == "S" || name == "V") {
            int h = colorParams.at(4)->value;
            if (h == 360) {
              h = 0;
            }
            Color hsv =
                Color(h, colorParams.at(5)->value, colorParams.at(6)->value);
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
        if (layer != -1) {
          Texture* t = texs.at(currentTexture);
          fx->undone = true;
          t->genTex();
        }
      } else {
        saveUndo();
      }
      doubleClickTimer = 0;
      textTypeTemp = this;
      textTypeFxTemp = fx;
      textTypeLayerTemp = layer;
      dragging = true;
      draggingParam = true;
      oldmX = mx;
      oldmY = my;
    }
  }
}

void Parameter::mouseMove(int mx, int my, int ex, int ey, int layer,
                          Effect* fx) {
  mx -= ex;
  my -= ey;
  if (ID == 0 || ID == 4) {
    if (dragging) {
      float dx = mx - (x + w / 2.0);
      float dy = my - (y + h / 2.0);
      value = atan2(-dy, -dx) / M_PI * 180.0 + 180.0;
      if (value == 360) {
        value = 0;
      }
      if (value < 0 || value > 360) {
        value = 0;
      }
    }
    if (dragging2) {
      float centerX = x + w / 2.0;
      float centerY = y + h / 2.0;
      float len = w / 2.0 * 0.8;
      float x1 = cos((value)*M_PI / 180.0) * len + centerX;  // 1, 1
      float y1 = sin((value)*M_PI / 180.0) * len + centerY;
      float x2 = cos((value + 120.0) * M_PI / 180.0) * len + centerX;  // 0, 1
      float y2 = sin((value + 120.0) * M_PI / 180.0) * len + centerY;
      float x3 = cos((value + 240.0) * M_PI / 180.0) * len + centerX;  // 0, 0
      float y3 = sin((value + 240.0) * M_PI / 180.0) * len + centerY;

      float oldX = mx - x3;
      float oldY = my - y3;
      float angle = 30.0 - value;
      float newX = (oldX * sin((120 - angle) * M_PI / 180.0) +
                    oldY * sin((120 - 90 - angle) * M_PI / 180.0)) /
                   sin(120 * M_PI / 180.0);
      float newY = (oldX * sin(angle * M_PI / 180.0) +
                    oldY * sin((90 + angle) * M_PI / 180.0)) /
                   sin(120 * M_PI / 180.0);

      float v1x = x1 - x2;
      float v1y = y1 - y2;
      float l1 = sqrt(v1x * v1x + v1y * v1y);
      float v2x = x2 - x3;
      float v2y = y2 - y3;
      float l2 = sqrt(v2x * v2x + v2y * v2y);

      newX /= l1;
      newY /= l2;

      value2 = newX / newY;
      value3 = newY;
      if (value2 < 0.0) {
        value2 = 0.0;
      } else if (value2 > 1.0) {
        value2 = 1.0;
      }
      if (value3 < 0.0) {
        value3 = 0.0;
      } else if (value3 > 1.0) {
        value3 = 1.0;
      }
      float H = value;
      float S = value2;
      float V = value3;
      Color* final = getPalColor(H, S, V);
      for (int col = 0; col < palette.size(); col++) {
        Color* color = palette.at(col);
        if (* final == *color) {
          value4 = col;
        }
      }
    }
    if ((dragging || dragging2) && layer == -1) {
      Color hsv = Color(value, value2 * 100, value3 * 100);
      Color rgb = HSVtoRGB(hsv.r, hsv.g, hsv.b);
      beforePaletteChange();
      colorParams.at(1)->value = rgb.r;
      colorParams.at(2)->value = rgb.g;
      colorParams.at(3)->value = rgb.b;
      colorParams.at(4)->value = value;
      colorParams.at(5)->value = value2 * 100;
      colorParams.at(6)->value = value3 * 100;
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
    if (ID == 0 && (dragging || dragging2)) {
      p->points.at(p->selectedPoint)->r = value;
      p->points.at(p->selectedPoint)->g = value2;
      p->points.at(p->selectedPoint)->b = value3;
      p->points.at(p->selectedPoint)->i = value4;
      updateGrad(p);
      if (layer != -1) {
        Texture* t = texs.at(currentTexture);
        fx->undone = true;
        t->genTex();
      }
    }
  } else if (ID == 7) {
    if (dragging) {
      points.at(selectedPoint)->a = (mx - 4) / 100.0;
      if (points.at(selectedPoint)->a > 1.0) {
        points.at(selectedPoint)->a = 1.0;
      } else if (points.at(selectedPoint)->a < 0.0) {
        points.at(selectedPoint)->a = 0.0;
      }
      updateGrad(this);
      if (layer != -1) {
        Texture* t = texs.at(currentTexture);
        fx->undone = true;
        t->genTex();
      }
    }
  } else if (ID == 1 || ID == 2 || ID == 3 || ID == 5 || ID == 6) {
    if (dragging) {
      float dist = 3;
      if (ID != 6) {
        if (s == NULL || s->s == NULL) {
          if (textType != this || (ID != 1 && ID != 2)) {
            if (my - oldmY < -dist) {  // vertical drag
              while (my - oldmY < -dist) {
                oldmY -= dist;
                if (value < value3 || value3 == -1) {
                  value++;
                }
              }
              updateDrag(layer, fx, this);
            } else if (my - oldmY > dist) {
              while (my - oldmY > dist) {
                oldmY += dist;
                if (value > value2 || value2 == -1) {
                  value--;
                }
              }
              updateDrag(layer, fx, this);
            }

            if (mx - oldmX < -dist) {  // horizontal drag
              while (mx - oldmX < -dist) {
                oldmX -= dist;
                if (value > value2 || value2 == -1) {
                  value--;
                }
              }
              updateDrag(layer, fx, this);
            } else if (mx - oldmX > dist) {
              while (mx - oldmX > dist) {
                oldmX += dist;
                if (value < value3 || value3 == -1) {
                  value++;
                }
              }
              updateDrag(layer, fx, this);
            }
          }
        }
      }
    }
  }
}

void Parameter::mouseUp(int mx, int my, int ex, int ey, int layer, Effect* fx) {
  mx -= ex;
  my -= ey;
  dragging = false;
  dragging2 = false;
  draggingParam = false;
}

Color getGrad(Parameter* param, int index) {
  Color ffinal(0, 0, 0);
  Color* final2 = NULL;
  int darkest = 0;
  float da = 1.0;
  float ca = 0.0;
  float a = index / 100.0;
  for (int pp = 0; pp < param->points.size(); pp++) {
    if (param->points.at(pp)->a <= da) {
      darkest = pp;
      da = param->points.at(pp)->a;
    }
    if (param->points.at(pp)->a >= ca && a > param->points.at(pp)->a) {
      ca = param->points.at(pp)->a;
      final2 = getPalColor(param->points.at(pp)->r, param->points.at(pp)->g,
                           param->points.at(pp)->b);
    }
  }
  if (final2 == NULL) {
    final2 =
        getPalColor(param->points.at(darkest)->r, param->points.at(darkest)->g,
                    param->points.at(darkest)->b);
  }
  ffinal.r = final2->r;
  ffinal.g = final2->g;
  ffinal.b = final2->b;
  return ffinal;
}
