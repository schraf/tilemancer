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

#ifndef TILEMANCER_RENDER_H
#define TILEMANCER_RENDER_H

#include "tilemancer/gl.h"

#include <string>

class Bezier;
class Color;

void renderRotIcon(int x, int y, int w, int h, GLuint tex, int frame);

void renderGlyph(int frame, float x, float y, float w, float h, GLuint tex,
                 int frameW, int frameH, float rot, float centerX,
                 float centerY, float alpha, GLuint texN, float depth,
                 float strength, bool flipX, bool flipY, int cutoff = -1,
                 int cutoff2 = -1, int cutoff3 = -1, int cutoff4 = -1);

void renderBezier(Bezier* b, GLuint tex);

void renderIcon(int x, int y, int w, int h, GLuint tex, int frame, int offX = 0,
                int offY = 0);

void renderColor(int x, int y, int w, int h, Color* c, int cutoff = -1,
                 int cutoff2 = -1, int cutoff3 = -1, int cutoff4 = -1);

void renderGradient(int x, int y, int w, int h, Color* c, Color* c2);

void renderSprite(int frame, float x, float y, float w, float h, GLuint tex,
                  float frameW, float frameH, float rot, float centerX,
                  float centerY, float alpha, GLuint texN, float depth,
                  float strength, bool flipX, bool flipY, float offX = 0,
                  float offY = 0, int cutoff = -1, int cutoff2 = -1,
                  int cutoff3 = -1, int cutoff4 = -1);

void renderNumber(int number, int x, int y, GLuint tex, bool percent,
                  bool alignRight, int cutoff = 0);

void renderWheel(int x, int y, int w, int h, float v);
void renderWheelTri(int x, int y, int w, int h, float v);
void renderWheelF(int x, int y, int w, int h, float v);
void renderWheelTriF(int x, int y, int w, int h, float v);

void renderUI(int x, int y, int w, int h, GLuint tex, int cutoff = -1,
              int cutoff2 = -1, int cutoff3 = -1, int cutoff4 = -1);

int renderText(std::string text, int x, int y, GLuint tex, bool alignRight,
               int cutoff = -1, int cutoff2 = -1, int cutoff3 = -1,
               int cutoff4 = -1, float alpha = 1.0);

int textW(const std::string& text, int x, int y, GLuint tex, bool alignRight);
int textH(const std::string& text, int x, int y, GLuint tex, bool alignRight);


#endif  // TILEMANCER_RENDER_H
