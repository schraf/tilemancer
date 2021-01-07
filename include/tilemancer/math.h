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

#ifndef TILEMANCER_MATH_H
#define TILEMANCER_MATH_H

float sign(float x1, float y1, float x2, float y2, float x3, float y3);

bool insideTri(float px, float py, float x1, float y1, float x2, float y2,
               float x3, float y3);

float dot(float x1, float y1, float x2, float y2);

#endif  // TILEMANCER_MATH_H
