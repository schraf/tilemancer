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

#include "tilemancer/effect.h"
#include "tilemancer/floatimage.h"
#include "tilemancer/globals.h"
#include "tilemancer/palette.h"
#include "tilemancer/parameter.h"
#include "tilemancer/socket.h"
#include "tilemancer/text.h"
#include "tilemancer/texture.h"
#include "tilemancer/tmlua.h"

void Effect::sideUpdate() {
  luaL_dofile(this->L, luafn.c_str());
  lua_settop(this->L, 0);
  lua_getglobal(this->L, "apply");
  lua_pcall(this->L, 0, 0, 0);
  loaded = true;
}

void Effect::clearTree() {
  cleared = true;
  done = false;
  doneTimer = 0;
  undone = false;
  for (int b = 0; b < outputs.size(); b++) {
    outputs.at(b)->texData.setAll(0);
  }
  for (int i = 0; i < texs.at(currentTexture)->fxs.size(); i++) {
    Effect* fx = texs.at(currentTexture)->fxs.at(i);
    for (int in = 0; in < fx->inputs.size(); in++) {
      Socket* input = fx->inputs.at(in);
      if (input->s != NULL && input->s->parent == this) {
        if (fx->cleared) {
          input->infloop = true;
        } else {
          input->infloop = false;
          fx->clearTree();
        }
      }
    }
  }
  cleared = false;
}

// LUA - C++ INTERACTION ------------------------

int Effect::setName(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 0) {
    name = lua_tostring(L, 1);
  }
  return 0;
}

int Effect::setDesc(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 0) {
    desc = lua_tostring(L, 1);
  }
  return 0;
}

int Effect::setSize(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 1) {
    w = lua_tonumber(L, 1);
    h = lua_tonumber(L, 2);
  }
  return 0;
}

int Effect::addInput(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 1) {
    Socket* s = new Socket();
    s->parent = this;
    s->y = lua_tonumber(L, 2) - 4;
    s->index = nextP;
    inputs.push_back(s);
    int height = 8;
    Text* t = new Text();
    t->name = lua_tostring(L, 1);
    t->y = lua_tonumber(L, 2) - height / 2.0;
    texts.push_back(t);
    nextP++;
  }
  return 0;
}

int Effect::addParameter(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 5) {
    int offset = 4;
    int height = 14;
    int type = 1;
    if (n > 6) {
      if (lua_toboolean(L, 7)) {
        type = 2;
      }
    }
    Parameter* p = new Parameter(
        type, lua_tostring(L, 1), offset, lua_tonumber(L, 3) - height / 2.0,
        w - offset * 2, height, (int)lua_tonumber(L, 4),
        (int)lua_tonumber(L, 5), (int)lua_tonumber(L, 6), lua_tostring(L, 2));
    p->index = nextP;
    params.push_back(p);
    nextP++;
  }
  return 0;
}

int Effect::addInputParameter(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 5) {
    Socket* s = new Socket();
    s->parent = this;
    s->y = lua_tonumber(L, 3) - 4;
    s->index = nextP;
    inputs.push_back(s);
    int offset = 4;
    int height = 14;
    int type = 1;
    if (n > 6) {
      if (lua_toboolean(L, 7)) {
        type = 2;
      }
    }
    Parameter* p = new Parameter(
        type, lua_tostring(L, 1), offset, lua_tonumber(L, 3) - height / 2.0,
        w - offset * 2, height, (int)lua_tonumber(L, 4),
        (int)lua_tonumber(L, 5), (int)lua_tonumber(L, 6), lua_tostring(L, 2));
    p->s = s;
    p->index = nextP;
    params.push_back(p);
    nextP++;
  }
  return 0;
}

int Effect::addOutput(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 0) {
    Socket* s = new Socket();
    s->parent = this;
    s->y = lua_tonumber(L, 1) - 4;
    outputs.push_back(s);
  }
  return 0;
}

int Effect::addCRamp(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 0) {
    int offset = 0;
    int height = 8;
    int s = 50;
    Parameter* p1 =
        new Parameter(7, "c", offset, lua_tonumber(L, 1) - height / 2.0 - 1,
                      108, height, 0, 0, 0, "");
    params.push_back(p1);
    Parameter* p2 =
        new Parameter(8, "c", 4, lua_tonumber(L, 1) - height / 2.0 + 8 + 4 - 1,
                      8, 8, 0, 0, 0, "");
    p2->p = p1;
    params.push_back(p2);
    Parameter* p3 = new Parameter(9, "c", 4 + 8 + 4,
                                  lua_tonumber(L, 1) - height / 2.0 + 8 + 4 - 1,
                                  8, 8, 0, 0, 0, "");
    p3->p = p1;
    params.push_back(p3);
    Parameter* pc = new Parameter(0, "c", 4 + 50 - s / 2.0,
                                  lua_tonumber(L, 1) - height / 2.0 + 8 + 4 - 1,
                                  s, s, 0, 0, 1, "");
    pc->p = p1;
    p1->p = pc;
    params.push_back(pc);
  }
  return 0;
}

int Effect::setPixel(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 5) {
    Socket* a = outputs.at((int)lua_tonumber(L, 1));
    Color b(lua_tonumber(L, 4) * 255, lua_tonumber(L, 5) * 255,
            lua_tonumber(L, 6) * 255);
    a->texData.setColor((int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3), b,
                        true);
  }
  return 0;
}

int Effect::getValue(lua_State* L) {
  float r = 0;
  float g = 0;
  float b = 0;
  int n = lua_gettop(L);
  if (n > 3) {
    for (int i = 0; i < params.size(); i++) {
      if (params.at(i)->index == (int)lua_tonumber(L, 1)) {
        r = params.at(i)->value / lua_tonumber(L, 4);
        g = params.at(i)->value / lua_tonumber(L, 4);
        b = params.at(i)->value / lua_tonumber(L, 4);
      }
    }
    for (int i = 0; i < inputs.size(); i++) {
      if (inputs.at(i)->index == (int)lua_tonumber(L, 1)) {
        Socket* a = inputs.at(i);
        if (a->s != NULL) {
          Color cc1 = a->s->texData.getColor((int)lua_tonumber(L, 2),
                                             (int)lua_tonumber(L, 3), true);
          Color cc2 = a->s->texData.getColor((int)lua_tonumber(L, 2) + 1,
                                             (int)lua_tonumber(L, 3), true);
          Color cc3 = a->s->texData.getColor((int)lua_tonumber(L, 2),
                                             (int)lua_tonumber(L, 3) + 1, true);
          Color cc4 = a->s->texData.getColor((int)lua_tonumber(L, 2) + 1,
                                             (int)lua_tonumber(L, 3) + 1, true);
          float alphaX = lua_tonumber(L, 2) - (int)lua_tonumber(L, 2);
          float alphaY = lua_tonumber(L, 3) - (int)lua_tonumber(L, 3);
          float ur = cc1.r * (1.0 - alphaX) + cc2.r * alphaX;
          float ug = cc1.g * (1.0 - alphaX) + cc2.g * alphaX;
          float ub = cc1.b * (1.0 - alphaX) + cc2.b * alphaX;
          float dr = cc3.r * (1.0 - alphaX) + cc4.r * alphaX;
          float dg = cc3.g * (1.0 - alphaX) + cc4.g * alphaX;
          float db = cc3.b * (1.0 - alphaX) + cc4.b * alphaX;
          r = ur * (1.0 - alphaY) + dr * alphaY;
          g = ug * (1.0 - alphaY) + dg * alphaY;
          b = ub * (1.0 - alphaY) + db * alphaY;
          r = r / 255.0;
          g = g / 255.0;
          b = b / 255.0;
        }
      }
    }
  }
  lua_pushnumber(L, r);
  lua_pushnumber(L, g);
  lua_pushnumber(L, b);
  return 3;
}

int Effect::finalize(lua_State* L) {
  for (int i = 0; i < texSizeX; i++) {
    for (int j = 0; j < texSizeY; j++) {
      Color c(0, 0, 0);
      if (inputs.at(0)->s != NULL) {
        c = inputs.at(0)->s->texData.getColor(i, j, true);
      }
      int value = c.r * 99.0 / 255.0;
      Color a = getGrad(params.at(0), value);
      outputs.at(0)->texData.setColor(i, j, a, true);
    }
  }
  return 0;
}

int Effect::getTileSize(lua_State* L) {
  lua_pushnumber(L, texSizeX);
  return 1;
}

int Effect::addNode(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 2) {
    std::string name = "";
    for (int i = 0; i < newEffects.size(); i++) {
      if (newEffects.at(i)->fxname == lua_tostring(L, 1)) {
        name = newEffects.at(i)->luafn;
      }
    }
    if (name.size() > 0) {
      Effect* fx = new Effect(name, lua_tostring(L, 1));
      fx->x += lua_tonumber(L, 2);
      fx->y += lua_tonumber(L, 3);
      presetFxs.push_back(fx);
    } else {
      if (!presetError) {
        errorMessage = "";
        presetError = true;
      }
      if (errorMessage.size() > 0) {
        errorMessage += "\n";
      }
      errorMessage +=
          "Unable to load \"" + (std::string)lua_tostring(L, 1) + "\"";
      errorTimer = 160;
    }
  }
  return 0;
}

int Effect::setParameter(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 2) {
    if (!presetError) {
      Effect* fx = presetFxs.at((int)lua_tonumber(L, 1));
      Parameter* param = fx->params.at((int)lua_tonumber(L, 2));
      param->value = (int)lua_tonumber(L, 3);
    }
  }
  return 0;
}

int Effect::addConnection(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 3) {
    if (!presetError) {
      Effect* fx1 = presetFxs.at((int)lua_tonumber(L, 1));
      Effect* fx2 = presetFxs.at((int)lua_tonumber(L, 3));
      Socket* output = fx1->outputs.at((int)lua_tonumber(L, 2));
      Socket* input = fx2->inputs.at((int)lua_tonumber(L, 4));
      input->s = output;
    }
  }
  return 0;
}

int my_rand(unsigned int* nextp) {  // custom RNG for consistency between OSes
  *nextp = *nextp * 1103515245 + 12345;
  return (unsigned int)(*nextp / 65536) % 32768;
}

int Effect::luaSeed(lua_State* L) {
  int n = lua_gettop(L);
  if (n > 0) {
    seed = (unsigned int)lua_tonumber(L, 1);
    my_rand(&seed);
  }
  return 0;
}

int Effect::luaRand(lua_State* L) {
  int rm = 32767;
  float r = my_rand(&seed) % rm / (float)rm;
  switch (lua_gettop(L)) {
    case 0: {
      lua_pushnumber(L, r);
      break;
    }
    case 1: {
      int u = (int)lua_tonumber(L, 1);
      luaL_argcheck(L, 1 <= u, 1, "interval is empty");
      lua_pushnumber(L, floor(r * u) + 1);
      break;
    }
    case 2: {
      int l = (int)lua_tonumber(L, 1);
      int u = (int)lua_tonumber(L, 2);
      luaL_argcheck(L, l <= u, 2, "interval is empty");
      lua_pushnumber(L, floor(r * (u - l + 1)) + l);
      break;
    }
    default:
      return luaL_error(L, "wrong number of arguments");
  }
  return 1;
}

void Effect::AbortLua(lua_State* L, lua_Debug* ar) {
  if (abort) {
    luaL_error(L, "Too Many Lines Error");
  }
}

Effect::~Effect() {
  for (int toDel = 0; toDel < params.size(); toDel++) {
    delete params.at(toDel);
  }
  for (int toDel = 0; toDel < inputs.size(); toDel++) {
    delete inputs.at(toDel);
  }
  for (int toDel = 0; toDel < outputs.size(); toDel++) {
    delete outputs.at(toDel);
  }
  for (int toDel = 0; toDel < texts.size(); toDel++) {
    delete texts.at(toDel);
  }
}

// wizardry shit
typedef int (Effect::*mem_func)(lua_State* L);
template <mem_func func>
int dispatch(lua_State* L) {
  Effect* ptr = *static_cast<Effect**>(lua_getextraspace(L));
  return ((*ptr).*func)(L);
}

// what the fuck?
typedef void (Effect::*mem_func2)(lua_State* L, lua_Debug* ar);
template <mem_func2 func2>
void dispatch2(lua_State* L, lua_Debug* ar) {
  Effect* ptr = *static_cast<Effect**>(lua_getextraspace(L));
  return ((*ptr).*func2)(L, ar);
}

// need to override ALL lua math functions just because of the custom RNG
static int math_abs(lua_State* L) {
  lua_pushnumber(L, l_mathop(fabs)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_sin(lua_State* L) {
  lua_pushnumber(L, l_mathop(sin)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_sinh(lua_State* L) {
  lua_pushnumber(L, l_mathop(sinh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_cos(lua_State* L) {
  lua_pushnumber(L, l_mathop(cos)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_cosh(lua_State* L) {
  lua_pushnumber(L, l_mathop(cosh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_tan(lua_State* L) {
  lua_pushnumber(L, l_mathop(tan)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_tanh(lua_State* L) {
  lua_pushnumber(L, l_mathop(tanh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_asin(lua_State* L) {
  lua_pushnumber(L, l_mathop(asin)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_acos(lua_State* L) {
  lua_pushnumber(L, l_mathop(acos)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_atan(lua_State* L) {
  lua_pushnumber(L, l_mathop(atan)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_atan2(lua_State* L) {
  lua_pushnumber(
      L, l_mathop(atan2)(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_ceil(lua_State* L) {
  lua_pushnumber(L, l_mathop(ceil)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_floor(lua_State* L) {
  lua_pushnumber(L, l_mathop(floor)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_fmod(lua_State* L) {
  lua_pushnumber(
      L, l_mathop(fmod)(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_modf(lua_State* L) {
  lua_Number ip;
  lua_Number fp = l_mathop(modf)(luaL_checknumber(L, 1), &ip);
  lua_pushnumber(L, ip);
  lua_pushnumber(L, fp);
  return 2;
}

static int math_sqrt(lua_State* L) {
  lua_pushnumber(L, l_mathop(sqrt)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_pow(lua_State* L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number y = luaL_checknumber(L, 2);
  lua_pushnumber(L, l_mathop(pow)(x, y));
  return 1;
}

static int math_log(lua_State* L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number res;
  if (lua_isnoneornil(L, 2))
    res = l_mathop(log)(x);
  else {
    lua_Number base = luaL_checknumber(L, 2);
    if (base == (lua_Number)10.0)
      res = l_mathop(log10)(x);
    else
      res = l_mathop(log)(x) / l_mathop(log)(base);
  }
  lua_pushnumber(L, res);
  return 1;
}

#if defined(LUA_COMPAT_LOG10)
static int math_log10(lua_State* L) {
  lua_pushnumber(L, l_mathop(log10)(luaL_checknumber(L, 1)));
  return 1;
}
#endif

static int math_exp(lua_State* L) {
  lua_pushnumber(L, l_mathop(exp)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_deg(lua_State* L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) / (M_PI / 180.0));
  return 1;
}

static int math_rad(lua_State* L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (M_PI / 180.0));
  return 1;
}

static int math_frexp(lua_State* L) {
  int e;
  lua_pushnumber(L, l_mathop(frexp)(luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}

static int math_ldexp(lua_State* L) {
  lua_Number x = luaL_checknumber(L, 1);
  int ep = (int)luaL_checknumber(L, 2);
  lua_pushnumber(L, l_mathop(ldexp)(x, ep));
  return 1;
}

static int math_min(lua_State* L) {
  int n = lua_gettop(L); /* number of arguments */
  lua_Number dmin = luaL_checknumber(L, 1);
  int i;
  for (i = 2; i <= n; i++) {
    lua_Number d = luaL_checknumber(L, i);
    if (d < dmin) dmin = d;
  }
  lua_pushnumber(L, dmin);
  return 1;
}

static int math_max(lua_State* L) {
  int n = lua_gettop(L); /* number of arguments */
  lua_Number dmax = luaL_checknumber(L, 1);
  int i;
  for (i = 2; i <= n; i++) {
    lua_Number d = luaL_checknumber(L, i);
    if (d > dmax) dmax = d;
  }
  lua_pushnumber(L, dmax);
  return 1;
}

static const luaL_Reg mathlib[] = {{"abs", math_abs},
                                   {"acos", math_acos},
                                   {"asin", math_asin},
                                   {"atan2", math_atan2},
                                   {"atan", math_atan},
                                   {"ceil", math_ceil},
                                   {"cosh", math_cosh},
                                   {"cos", math_cos},
                                   {"deg", math_deg},
                                   {"exp", math_exp},
                                   {"floor", math_floor},
                                   {"fmod", math_fmod},
                                   {"frexp", math_frexp},
                                   {"ldexp", math_ldexp},
#if defined(LUA_COMPAT_LOG10)
                                   {"log10", math_log10},
#endif
                                   {"log", math_log},
                                   {"max", math_max},
                                   {"min", math_min},
                                   {"modf", math_modf},
                                   {"pow", math_pow},
                                   {"rad", math_rad},
                                   {"random", &dispatch<&Effect::luaRand>},
                                   {"randomseed", &dispatch<&Effect::luaSeed>},
                                   {"sinh", math_sinh},
                                   {"sin", math_sin},
                                   {"sqrt", math_sqrt},
                                   {"tanh", math_tanh},
                                   {"tan", math_tan},
                                   {NULL, NULL}};

LUAMOD_API int luaopen_math2(lua_State* L) {
  luaL_newlib(L, mathlib);
  lua_pushnumber(L, M_PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, HUGE_VAL);
  lua_setfield(L, -2, "huge");
  return 1;
}

LUALIB_API void luaL_requiref2(lua_State* L, const char* modname,
                               lua_CFunction openf, int glb) {
  lua_pushcfunction(L, openf);
  lua_pushstring(L, modname); /* argument to open function */
  lua_call(L, 1, 1);          /* open module */
  luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");
  lua_pushvalue(L, -2);         /* make copy of module (call result) */
  lua_setfield(L, -2, modname); /* _LOADED[modname] = module */
  lua_pop(L, 1);                /* remove _LOADED table */
  if (glb) {
    lua_pushvalue(L, -1);      /* copy of 'mod' */
    lua_setglobal(L, modname); /* _G[modname] = module */
  }
}

Effect::Effect(std::string luafn, std::string fxname, bool preset) {
  this->isPreset = preset;
  this->fxname = fxname;
  this->nextP = 0;
  this->cleared = false;
  this->abort = false;
  this->deleted = false;
  this->loading = false;
  this->loaded = false;
  this->done = false;
  this->undone = true;
  this->L = luaL_newstate();
  luaL_openlibs(this->L);
  *static_cast<Effect**>(lua_getextraspace(L)) = this;
  if (!isPreset) {
    lua_register(this->L, "setName", &dispatch<&Effect::setName>);
    lua_register(this->L, "setDesc", &dispatch<&Effect::setDesc>);
    lua_register(this->L, "setSize", &dispatch<&Effect::setSize>);
    lua_register(this->L, "addInput", &dispatch<&Effect::addInput>);
    lua_register(this->L, "addParameter", &dispatch<&Effect::addParameter>);
    lua_register(this->L, "addInputParameter",
                 &dispatch<&Effect::addInputParameter>);
    lua_register(this->L, "addOutput", &dispatch<&Effect::addOutput>);
    lua_register(this->L, "getTileSize", &dispatch<&Effect::getTileSize>);
    lua_register(this->L, "getValue", &dispatch<&Effect::getValue>);
    lua_register(this->L, "setPixel", &dispatch<&Effect::setPixel>);

    lua_register(this->L, "addCRamp", &dispatch<&Effect::addCRamp>);
    lua_register(this->L, "colorize", &dispatch<&Effect::finalize>);
  } else {
    lua_register(this->L, "setName", &dispatch<&Effect::setName>);
    lua_register(this->L, "setDesc", &dispatch<&Effect::setDesc>);
    lua_register(this->L, "addNode", &dispatch<&Effect::addNode>);
    lua_register(this->L, "setParameter", &dispatch<&Effect::setParameter>);
    lua_register(this->L, "addConnection", &dispatch<&Effect::addConnection>);
  }

  luaL_requiref2(this->L, "math", luaopen_math2, 1);

  lua_sethook(this->L, &dispatch2<&Effect::AbortLua>, LUA_MASKCOUNT, 2);
  this->luafn = luafn;
  name = "";
  desc = "";
  sx = 0;
  sy = 0;
  w = 32;
  h = 32;
  r = 0;
  luaL_dofile(this->L, luafn.c_str());
  lua_settop(this->L, 0);
  lua_getglobal(this->L, "init");
  lua_pcall(this->L, 0, 0, 0);
  x = -w / 2 + screenW / 2.0 - nodeCX;
  y = -h / 2 + screenH / 2.0 - nodeCY;
}
