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

#include "tilemancer/stringutils.h"
#include <sstream>
#include <cstring>

std::vector<std::string>& split(const std::string& s, char delim,
                                std::vector<std::string>& elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

void fixPath(char* path) {
  char buf[1024];
  int j = 0;

  for (int i = 0; path[i] != '\0'; ++i) {
    if (path[i] == '\\') {
      buf[j++] = '/';
    } else {
      buf[j++] = path[i];
    }
  }

  buf[j] = 0;
  strncpy(path, buf, j);
}
