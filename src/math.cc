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

float sign(float x1, float y1, float x2, float y2, float x3, float y3) {
  return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}

bool insideTri(float px, float py, float x1, float y1, float x2, float y2,
               float x3, float y3) {
  bool b1, b2, b3;
  b1 = sign(px, py, x1, y1, x2, y2) < 0.0f;
  b2 = sign(px, py, x2, y2, x3, y3) < 0.0f;
  b3 = sign(px, py, x3, y3, x1, y1) < 0.0f;
  if ((b1 == b2) && (b2 == b3)) {
    return true;
  } else {
    return false;
  }
}

float dot(float x1, float y1, float x2, float y2) { return x1 * x2 + y1 * y2; }
