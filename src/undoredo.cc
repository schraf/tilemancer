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

#include "tilemancer/undoredo.h"
#include "tilemancer/globals.h"
#include "tilemancer/saveload.h"

void undo() {
  if (listUndo.size() > 0) {
    listRedo.push_back(saveStuff());
    loadStuff(listUndo.back(), false);
    listUndo.pop_back();
  }
}

void redo() {
  if (listRedo.size() > 0) {
    // cout << listRedo.size() << endl;
    listUndo.push_back(saveStuff());
    loadStuff(listRedo.back(), false);
    listRedo.pop_back();
  }
}

void saveUndo() {
  listUndo.push_back(saveStuff());
  listRedo.clear();
}
