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

#include "tilemancer/bezier.h"

#include <vector>

Bezier::Bezier() {}

void Bezier::create() {
  glGenBuffers(1, &bVbo);
  update();
}

bPoint Bezier::getPoint(float t) const {
  float x = powf(1 - t, 3) * P0.x + 3 * powf(1 - t, 2) * t * P1.x +
            3 * (1 - t) * powf(t, 2) * P2.x + powf(t, 3) * P3.x;
  float y = powf(1 - t, 3) * P0.y + 3 * powf(1 - t, 2) * t * P1.y +
            3 * (1 - t) * powf(t, 2) * P2.y + powf(t, 3) * P3.y;
  return bPoint(x, y);
}

bPoint Bezier::getTangent(float t) const {
  float x = 3 * powf(1 - t, 2) * (P1.x - P0.x) +
            6 * (1 - t) * t * (P2.x - P1.x) + 3 * powf(t, 2) * (P3.x - P2.x);
  float y = 3 * powf(1 - t, 2) * (P1.y - P0.y) +
            6 * (1 - t) * t * (P2.y - P1.y) + 3 * powf(t, 2) * (P3.y - P2.y);
  float l = sqrtf(x * x + y * y);
  x /= l;
  y /= l;
  // cout << x << " " << y << endl;
  return bPoint(x, y);
}

void Bezier::update() {
  float segs = 1.0f / d;
  std::vector<float> data;
  for (int i = 0; i < d; i++) {
    bPoint a = getPoint(i * segs);
    bPoint b = getPoint((i + 1) * segs);
    bPoint ta = getTangent(i * segs);
    bPoint tb = getTangent((i + 1) * segs);
    bPoint pa = bPoint(ta.y, -ta.x);
    bPoint pb = bPoint(tb.y, -tb.x);
    float w = 1;
    data.push_back(a.x - pa.x * w);
    data.push_back(a.y - pa.y * w);
    data.push_back(0);
    data.push_back(0);
    data.push_back(b.x - pb.x * w);
    data.push_back(b.y - pb.y * w);
    data.push_back(0);
    data.push_back(0);
    data.push_back(b.x + pb.x * w);
    data.push_back(b.y + pb.y * w);
    data.push_back(0);
    data.push_back(0);

    data.push_back(a.x - pa.x * w);
    data.push_back(a.y - pa.y * w);
    data.push_back(0);
    data.push_back(0);
    data.push_back(b.x + pb.x * w);
    data.push_back(b.y + pb.y * w);
    data.push_back(0);
    data.push_back(0);
    data.push_back(a.x + pa.x * w);
    data.push_back(a.y + pa.y * w);
    data.push_back(0);
    data.push_back(0);
  }
  glBindBuffer(GL_ARRAY_BUFFER, bVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0],
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
