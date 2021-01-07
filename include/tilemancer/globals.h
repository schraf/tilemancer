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

#ifndef TILEMANCER_GLOBALS_H
#define TILEMANCER_GLOBALS_H

#include "tilemancer/color.h"

#include <string>
#include <vector>

extern "C" {
struct lua_State;
};

class Texture;
class Effect;
class BrowserFile;
class Parameter;
class Socket;

const int Vmajor = 0;
const int Vminor = 2;
const int Vrevision = 1;

extern double dt;
extern double currentTime;
extern double lastTime;
extern double accumulator;
extern unsigned int fps;
extern int fpsTimer;

extern int texSizeX;
extern int texSizeY;
extern int screenW;
extern int screenH;
extern int displayW;
extern int displayH;
extern float camX;
extern float camY;
extern float nodeCSX;
extern float nodeCSY;
extern float nodeCX;
extern float nodeCY;
extern bool shiftIsDown;
extern bool draggingCam;
extern bool draggingCam2;
extern float camOffset;
extern float camOffsetSpeed;
extern float camOffsetMagnitude;
extern bool camPositive;
extern float camRot;
extern int camXFinal;
extern int camYFinal;
extern int zoom;
extern int draggedLayer;
extern bool draggingNode;
extern int draggedNode;
extern bool draggingSocket;
extern int draggedSocket;
extern float screenScale;
extern int logoTimer;
extern int logoTimerMax;
extern std::string toolTip;
extern std::string errorMessage;
extern int errorTimer;
extern std::vector<float> data;
extern std::vector<Color*> palette;
extern Color rampA;
extern Color rampB;
extern int selectedColor;
extern std::vector<Parameter*> colorParams;
extern Parameter* textType;
extern Effect* textTypeFx;
extern int textTypeLayer;
extern Parameter* textTypeTemp;
extern Effect* textTypeFxTemp;
extern int textTypeLayerTemp;
extern int blinkTimer;
extern std::vector<Texture*> texs;
extern std::vector<Effect*> newEffects;
extern int collH;
extern int expH;
extern int currentTexture;
extern Socket* currentSocket;
extern float barX;
extern float barXRight;
extern float barY;
extern float barY2;
extern float barY3;
extern float barY4;
extern float barY5;
extern float layersScroll;
extern float toolsScroll;
extern float palScroll;
extern float texScroll;
extern float newEScroll;
extern float layersScrollH;
extern bool draggingParam;
extern bool draggingFX;
extern bool draggingUI;
extern bool draggingSBar;
extern int sBarDrag;
extern float sRatio;
extern int rotTimer;
extern int UIdragType;
extern int UIdragRange;
extern float mouseOX;
extern float mouseOY;
extern float mouseX;
extern float mouseY;
// extern bool gameStarted;
extern int view;
extern int doubleClickTimer;
extern int typeTimer;
extern Socket* preview;
extern int previewTimer;
extern std::vector<std::string> listUndo;
extern std::vector<std::string> listRedo;
extern std::string currentDir;
extern std::string filenameB;
extern std::string lastPalDir;
extern std::string lastPalName;
extern std::string lastSaveDir;
extern std::string lastSaveName;
extern std::string lastTexDir;
extern std::string lastTexName;

extern lua_State* L;

#endif  // TILEMANCER_GLOBALS_H
