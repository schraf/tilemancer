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

#ifndef TILEMANCER_PARAMETER_H
#define TILEMANCER_PARAMETER_H

#include <string>
#include <vector>
#include "tilemancer/color.h"

class Socket;
class CPoint;
class Effect;

class Parameter {
 public:
  Parameter(int ID, std::string name, float x, float y, float w, float h,
            int value, int value2, int value3, std::string tt);
  ~Parameter();

  void render(int ex, int ey);
  void mouseDown(int mx, int my, int ex, int ey, int layer, Effect* fx);
  void mouseMove(int mx, int my, int ex, int ey, int layer, Effect* fx);
  void mouseUp(int mx, int my, int ex, int ey, int layer, Effect* fx);

  int ID;
  std::string tt;
  std::string name;
  Socket* s;
  std::vector<CPoint*> points;
  Parameter* p;
  int index;
  int selectedPoint;
  float x;
  float y;
  float w;
  float h;
  float resetValue;
  std::string typing;
  float value;
  float value2;
  float value3;
  float value4;
  bool dragging;
  bool dragging2;
  float oldmX;
  float oldmY;
};

Color getGrad(Parameter* param, int index);

#endif  // TILEMANCER_PARAMETER_H
