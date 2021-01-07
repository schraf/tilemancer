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

#ifndef TILEMANCER_OS_H
#define TILEMANCER_OS_H

enum { Windows = 0x1, Apple = 0x2, Linux = 0x4, Unix = Apple | Linux };

#ifdef _WIN32

#include "os_windows.h"

#elif defined(__APPLE__)

#include "os_osx.h"

#elif defined(__linux__)

#include "os_linux.h"

#else

#error "Unsupported OS"

#endif

#endif  // TILEMANCER_OS_H