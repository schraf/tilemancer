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

#include "tilemancer/color.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

Color::Color(int ar, int ag, int ab)
    : r(ar), g(ag), b(ab), a(255), disabled(false) {}

bool Color::operator==(const Color& rhs) const {
  return r == rhs.r && g == rhs.g && b == rhs.b;
}

Color HSVtoRGB(float H, float S, float V) {
  V /= 100;
  S /= 100;
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

  Color rgb = Color(R, G, B);
  return rgb;
}

Color RGBtoHSV(float R, float G, float B) {
  float R1 = R / 255.0;
  float G1 = G / 255.0;
  float B1 = B / 255.0;
  float cMax = std::max(std::max(R1, G1), B1);
  float cMin = std::min(std::min(R1, G1), B1);
  float delta = cMax - cMin;

  float H;
  if (delta == 0) {
    H = 0;
  } else if (cMax == R1) {
    H = 60 * fmod(((G1 - B1) / delta), 6);
  } else if (cMax == G1) {
    H = 60 * ((B1 - R1) / delta + 2);
  } else if (cMax == B1) {
    H = 60 * ((R1 - G1) / delta + 4);
  }

  while (H > 360) {
    H -= 360;
  }
  while (H < 0) {
    H += 360;
  }

  float S;
  if (cMax == 0) {
    S = 0;
  } else {
    S = delta / cMax;
  }
  S *= 100;

  float V = cMax * 100;

  Color hsv = Color(H, S, V);
  return hsv;
}

double getLuminance(double R, double G, double B) {
  double R1;
  double G1;
  double B1;
  // cout << "l" << R << " " << G << " " << B << endl;
  if (R <= 0.03928) {
    R1 = R / 12.92;
  } else {
    R1 = pow(((R + 0.055) / 1.055), 2.4);
  }
  if (G <= 0.03928) {
    G1 = G / 12.92;
  } else {
    G1 = pow(((G + 0.055) / 1.055), 2.4);
  }
  if (B <= 0.03928) {
    B1 = B / 12.92;
  } else {
    B1 = pow(((B + 0.055) / 1.055), 2.4);
  }
  double L = 0.2126 * R1 + 0.7152 * G1 + 0.0722 * B1;
  return L;
}

double getContrast(float y1, float y2) {
  if (y1 < y2) {
    float y3 = y2;
    y2 = y1;
    y1 = y3;
  }
  return (y1 + 0.05) / (y2 + 0.05);
}

double getContrast(Color a, Color b) {
  float y1 = getLuminance(a.r, a.g, a.b);
  float y2 = getLuminance(b.r, b.g, b.b);
  if (y1 < y2) {
    float y3 = y2;
    y2 = y1;
    y1 = y3;
  }
  return (y1 + 0.05) / (y2 + 0.05);
}
