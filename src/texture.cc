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

#include "tilemancer/texture.h"
#include "tilemancer/effect.h"
#include "tilemancer/socket.h"

#include <thread>

Texture::~Texture() {}

void Texture::genTex(bool first) {
  if (first) {  // abort all before proceeding
    abort = true;
    for (int i = 0; i < fxs.size(); i++) {
      if (fxs.at(i)->loading) {
        fxs.at(i)->abort = true;
      }
    }
  } else {
    for (int i = 0; i < fxs.size(); i++) {
      if (!fxs.at(i)->done) {
        bool ready = true;
        for (int b = 0; b < fxs.at(i)->inputs.size(); b++) {
          if (fxs.at(i)->inputs.at(b)->s != NULL) {
            if (!fxs.at(i)->inputs.at(b)->s->parent->done) {
              ready = false;
            }
          }
        }
        if (ready) {
          if (!fxs.at(i)->loading) {
            fxs.at(i)->loading = true;
            fxs.at(i)->side = std::thread(&Effect::sideUpdate, fxs.at(i));
          }
        }
      }
    }
  }
}
