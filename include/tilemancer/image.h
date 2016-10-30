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

#ifndef TILEMANCER_IMAGE_H
#define TILEMANCER_IMAGE_H

#include <string>
#include <vector>

// horrible image load result, but I hate memory managment so sue me
class ImageLoadResult {
 public:
  std::vector<unsigned char> components;
  int width;
  int height;
  bool has_alpha;

  std::string error;
};

enum class AlphaLoad {
  Remove, Include
};

enum class AlphaSave {
  Has, HasNot
};

ImageLoadResult LoadImage(const std::string& path, AlphaLoad alpha);
std::string SaveImage(const std::string& path, const std::vector<unsigned char>& pixels, int width, int height, AlphaSave alpha);

#endif  // TILEMANCER_IMAGE_H
