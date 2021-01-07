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

#include "tilemancer/globals.h"

double dt = 1.0 / 60.0;
double currentTime;
double lastTime;
double accumulator;
unsigned int fps;
int fpsTimer;

int texSizeX;
int texSizeY;
int screenW;
int screenH;
int displayW;
int displayH;
float camX;
float camY;
float nodeCSX;
float nodeCSY;
float nodeCX;
float nodeCY;
bool shiftIsDown = false;
bool draggingCam = false;
bool draggingCam2 = false;
float camOffset;
float camOffsetSpeed;
float camOffsetMagnitude;
bool camPositive;
float camRot;
int camXFinal;
int camYFinal;
int zoom = 1;
int draggedLayer;
bool draggingNode;
int draggedNode;
bool draggingSocket;
int draggedSocket;
float screenScale = 2.0;
int logoTimer;
int logoTimerMax = 150;
std::string toolTip;
std::string errorMessage;
int errorTimer;
std::vector<float> data;
std::vector<Color*> palette;
Color rampA(254, 237, 186);
Color rampB(54, 42, 96);
int selectedColor = 0;
std::vector<Parameter*> colorParams;
Parameter* textType;
Effect* textTypeFx;
int textTypeLayer;
Parameter* textTypeTemp;
Effect* textTypeFxTemp;
int textTypeLayerTemp;
int blinkTimer;
std::vector<Texture*> texs;
std::vector<Effect*> newEffects;
int collH = 18;
int expH = 92;
int currentTexture;
Socket* currentSocket;
float barX;
float barXRight;
float barY;
float barY2;
float barY3;
float barY4;
float barY5;
float layersScroll;
float toolsScroll;
float palScroll;
float texScroll;
float newEScroll;
float layersScrollH;
bool draggingParam;
bool draggingFX;
bool draggingUI;
bool draggingSBar;
int sBarDrag;
float sRatio;
int rotTimer;
int UIdragType;
int UIdragRange = 5;
float mouseOX;
float mouseOY;
float mouseX;
float mouseY;
int view = 0;
int doubleClickTimer;
int typeTimer;
Socket* preview;
int previewTimer;
std::vector<std::string> listUndo;
std::vector<std::string> listRedo;
std::string currentDir;
std::string filenameB;
std::string lastPalDir;
std::string lastPalName;
std::string lastSaveDir;
std::string lastSaveName;
std::string lastTexDir;
std::string lastTexName;
lua_State* L;

