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

#ifndef TILEMANCER_COLOR_H
#define TILEMANCER_COLOR_H

class Color {
 public:
  Color(int r, int g, int b);
  bool operator==(const Color& rhs) const;

  int r;
  int g;
  int b;
  int a;
  bool disabled;
};

Color HSVtoRGB(float H, float S, float V);
Color RGBtoHSV(float R, float G, float B);

double getLuminance(double R, double G, double B);
double getContrast(float y1, float y2);
double getContrast(Color a, Color b);

#endif  // TILEMANCER_COLOR_H
