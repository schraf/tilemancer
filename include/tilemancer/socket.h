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

#ifndef TILEMANCER_SOCKET_H
#define TILEMANCER_SOCKET_H

#include <vector>
#include <string>

#include "tilemancer/floatimage.h"
#include "tilemancer/gl.h"

class Bezier;
class Effect;

class Socket {
 public:
  Socket();
  ~Socket();

  float y;
  bool snapped;
  bool infloop;
  Socket* s;
  Bezier* b;
  int index;
  float px;
  float py;
  GLuint texture;
  int futureN;
  int futureS;
  Effect* parent;
  FloatImage texData;
  std::string lastTexDir;
  std::string lastTexName;
};

void exportTexSingle(const std::string& dir);

#endif  // TILEMANCER_SOCKET_H
