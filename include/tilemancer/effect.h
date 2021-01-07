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

#ifndef TILEMANCER_EFFECT_H
#define TILEMANCER_EFFECT_H

class Socket;
class Parameter;
class Text;

#include <string>
#include <thread>
#include <vector>

#include "tilemancer/tmlua.h"

class Effect {
 public:
  Effect(std::string luafn, std::string fxname, bool preset = false);
  ~Effect();

  lua_State* L;
  unsigned int seed;
  std::thread side;

  void sideUpdate();
  void clearTree();

  std::vector<Socket*> inputs;
  std::vector<Socket*> outputs;
  std::vector<Parameter*> params;
  std::vector<Text*> texts;
  std::vector<Effect*> presetFxs;
  std::string luafn;
  std::string fxname;
  std::string name;
  std::string desc;
  int nextP;
  bool cleared;
  bool presetError;
  bool abort;
  bool deleted;
  bool loading;
  bool done;
  bool undone;
  int doneTimer;
  bool loaded;
  bool isPreset;
  float x;
  float y;
  float sx;
  float sy;
  float w;
  float h;
  float r;

  int setName(lua_State* L);
  int setDesc(lua_State* L);
  int setSize(lua_State* L);
  int addInput(lua_State* L);
  int addParameter(lua_State* L);
  int addInputParameter(lua_State* L);
  int addOutput(lua_State* L);
  int getTileSize(lua_State* L);
  int getValue(lua_State* L);
  int setPixel(lua_State* L);
  int addCRamp(lua_State* L);
  int finalize(lua_State* L);

  int luaSeed(lua_State* L);
  int luaRand(lua_State* L);

  int addNode(lua_State* L);
  int setParameter(lua_State* L);
  int addConnection(lua_State* L);
  void AbortLua(lua_State* L, lua_Debug* ar);
};

#endif  // TILEMANCER_EFFECT_H
