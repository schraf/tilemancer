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

#ifndef TILEMANCER_BEZIER_H
#define TILEMANCER_BEZIER_H

#include "tilemancer/bpoint.h"
#include "tilemancer/gl.h"

class Bezier {
 public:
  Bezier();
  bPoint getTangent(float t) const;
  bPoint getPoint(float t) const;
  void create();
  void update();
  int d = 100;
  GLuint bVbo;
  bPoint P0;
  bPoint P1;
  bPoint P2;
  bPoint P3;
};

#endif  // TILEMANCER_BEZIER_H
