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

#ifndef TILEMANCER_TEXTURE_H
#define TILEMANCER_TEXTURE_H

#include <string>
#include <vector>

class Effect;

class Texture {
 public:
  ~Texture();
  void genTex(bool first = true);
  bool done;
  int doneTimer;
  bool abort;
  std::vector<Effect*> fxs;
  std::string lastTexDir;
  std::string lastTexName;
};

#endif  // TILEMANCER_TEXTURE_H
