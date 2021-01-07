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

#include "tilemancer/saveload.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "tilemancer/cpoint.h"
#include "tilemancer/effect.h"
#include "tilemancer/globals.h"
#include "tilemancer/graphics_globals.h"
#include "tilemancer/palette.h"
#include "tilemancer/parameter.h"
#include "tilemancer/socket.h"
#include "tilemancer/stringutils.h"
#include "tilemancer/texture.h"

std::string saveStuff() {
  std::ostringstream out;
  out << "Tilemancer " << Vmajor << "." << Vminor << "." << Vrevision
      << std::endl;
  out << std::endl;
  for (int i = 0; i < palette.size(); i++) {
    Color* c = palette.at(i);
    out << "Color " << c->r << " " << c->g << " " << c->b << std::endl;
  }
  out << "PaletteSave1 " << lastPalDir << std::endl;
  out << "PaletteSave2 " << lastPalName << std::endl;
  out << "FileSave1 " << lastSaveDir << std::endl;
  out << "FileSave2 " << lastSaveName << std::endl;
  out << "Indexed " << colorParams.at(7)->value << std::endl;
  out << "Size " << colorParams.at(8)->value << std::endl;
  out << "Camera " << nodeCX << " " << nodeCY << std::endl;
  Texture* t = texs.at(currentTexture);
  for (int i = 0; i < t->fxs.size(); i++) {
    Effect* fx = t->fxs.at(i);
    out << "Node {" << std::endl;
    out << "Name " << fx->fxname << std::endl;
    out << "Position " << fx->x << " " << fx->y << std::endl;
    for (int p = 0; p < fx->params.size(); p++) {
      Parameter* param = fx->params.at(p);
      if (param->ID == 7) {
        out << "Ramp {" << std::endl;
        for (int p = 0; p < param->points.size(); p++) {
          out << "Point " << param->points.at(p)->a * 100 << " "
              << param->points.at(p)->r << " " << param->points.at(p)->g << " "
              << param->points.at(p)->b << std::endl;
        }
        out << "}" << std::endl;
      } else {
        out << "Parameter " << param->value << " " << param->value2 << " "
            << param->value3 << " " << param->value4 << std::endl;
      }
    }
    for (int in = 0; in < fx->inputs.size(); in++) {
      Socket* input = fx->inputs.at(in);
      if (input->s != NULL) {
        int nodeIndex = find(t->fxs.begin(), t->fxs.end(), input->s->parent) -
                        t->fxs.begin();
        int socketIndex = find(input->s->parent->outputs.begin(),
                               input->s->parent->outputs.end(), input->s) -
                          input->s->parent->outputs.begin();
        out << "Connection " << in << " " << nodeIndex << " " << socketIndex
            << std::endl;
      }
    }
    out << "}" << std::endl;
  }
  return out.str();
}

void loadStuff(std::string str, bool newFile) {
  std::cout << "Loading stuff " << str << " " << newFile << std::endl;
  std::istringstream fin(str);
  if (fin.good()) {
    std::string line;
    getline(fin, line);
    std::vector<std::string> a = split(line, ' ');
    bool error = false;
    errorMessage = "";
    if (a.at(0) == "Tilemancer") {  // valid
      std::vector<std::string> b = split(a.at(1), '.');
      int fileMj = atoi(b.at(0).c_str());
      int fileMn = atoi(b.at(1).c_str());
      int fileRv = atoi(b.at(2).c_str());
      int comp = 0;  // equal
      if (fileMj > Vmajor) {
        comp = 1;  // opening file saved on newer version
      } else if (fileMj < Vmajor) {
        comp = -1;  // opening old save file
      } else {
        if (fileMn > Vminor) {
          comp = 1;  // opening file saved on newer version
        } else if (fileMn < Vminor) {
          comp = -1;  // opening old save file
        } else {
          if (fileRv > Vrevision) {
            comp = 1;  // opening file saved on newer version
          } else if (fileRv < Vrevision) {
            comp = -1;  // opening old save file
          } else {
            // equal
          }
        }
      }
      for (int toDel = 0; toDel < texs.size(); toDel++) {
        delete texs.at(toDel);
      }
      texs.clear();
      Texture* t = new Texture();
      texs.push_back(t);
      if (newFile) {
        currentTexture = 0;
      }
      for (int toDel = 0; toDel < palette.size(); toDel++) {
        delete palette.at(toDel);
      }
      palette.clear();
      if (newFile) {
        selectedColor = 0;
      }
      getline(fin, line);
      while (getline(fin, line)) {
        a.clear();
        a = split(line, ' ');
        if (a.size() > 0) {
          if (a.at(0) == "Color") {
            Color* c = new Color(atoi(a.at(1).c_str()), atoi(a.at(2).c_str()),
                                 atoi(a.at(3).c_str()));
            palette.push_back(c);
          }
          if (a.at(0) == "PaletteSave1") {
            if (a.size() > 1) {
              lastPalDir = a.at(1);
            } else {
              lastPalDir = "";
            }
          }
          if (a.at(0) == "PaletteSave2") {
            if (a.size() > 1) {
              lastPalName = a.at(1);
            } else {
              lastPalName = "";
            }
          }
          if (a.at(0) == "FileSave1") {
            if (a.size() > 1) {
              lastSaveDir = a.at(1);
            } else {
              lastSaveDir = "";
            }
          }
          if (a.at(0) == "FileSave2") {
            if (a.size() > 1) {
              lastSaveName = a.at(1);
            } else {
              lastSaveName = "";
            }
          }
          if (a.at(0) == "Indexed") {
            colorParams.at(7)->value = atoi(a.at(1).c_str());
          }
          if (a.at(0) == "Size") {
            colorParams.at(8)->value = atoi(a.at(1).c_str());
            texSizeX = colorParams.at(8)->value;
            texSizeY = texSizeX;
          }
          if (a.at(0) == "Camera") {
            nodeCX = atoi(a.at(1).c_str());
            nodeCY = atoi(a.at(2).c_str());
            nodeCSX = 0;
            nodeCSY = 0;
          }
          if (a.at(0) == "Node") {
            getline(fin, line);
            a.clear();
            a = split(line, ' ');
            std::string name = "";
            for (int i = 0; i < newEffects.size(); i++) {
              if (newEffects.at(i)->fxname == a.at(1)) {
                name = newEffects.at(i)->luafn;
              }
            }
            if (name.size() > 0) {
              Effect* fx = new Effect(name, a.at(1));
              bool nodeFinished = false;
              int param = 0;
              while (!nodeFinished) {
                getline(fin, line);
                a.clear();
                a = split(line, ' ');
                if (a.at(0) == "}") {
                  nodeFinished = true;
                  texs.at(currentTexture)->fxs.push_back(fx);
                }
                if (a.at(0) == "Position") {
                  fx->x = atof(a.at(1).c_str());
                  fx->y = atof(a.at(2).c_str());
                }
                if (a.at(0) == "Ramp") {
                  fx->params.at(param)->points.clear();
                  bool rampDone = false;
                  while (!rampDone) {
                    getline(fin, line);
                    a.clear();
                    a = split(line, ' ');
                    if (a.at(0) == "}") {
                      rampDone = true;
                      param++;
                    }
                    if (a.at(0) == "Point") {
                      CPoint* cp = new CPoint();
                      cp->a = atoi(a.at(1).c_str()) / 100.0;
                      cp->r = atof(a.at(2).c_str());
                      cp->g = atof(a.at(3).c_str());
                      cp->b = atof(a.at(4).c_str());
                      fx->params.at(param)->points.push_back(cp);
                    }
                  }
                }
                if (a.at(0) == "Parameter") {
                  fx->params.at(param)->value = atoi(a.at(1).c_str());
                  fx->params.at(param)->value2 = atoi(a.at(2).c_str());
                  fx->params.at(param)->value3 = atoi(a.at(3).c_str());
                  fx->params.at(param)->value4 = atoi(a.at(4).c_str());
                  param++;
                }
                if (a.at(0) == "Connection") {
                  fx->inputs.at(atoi(a.at(1).c_str()))->futureN =
                      atoi(a.at(2).c_str());
                  fx->inputs.at(atoi(a.at(1).c_str()))->futureS =
                      atoi(a.at(3).c_str());
                }
              }
            } else {
              if (errorMessage.size() > 0) {
                errorMessage += "\n";
              }
              errorMessage += "Unable to load \"" + a.at(1) + "\"";
              errorTimer = 160;
              error = true;
            }
          }
        }
      }
      for (int i = 0; i < texs.at(currentTexture)->fxs.size(); i++) {
        Effect* fx = texs.at(currentTexture)->fxs.at(i);
        for (int in = 0; in < fx->inputs.size(); in++) {
          Socket* input = fx->inputs.at(in);
          if (input->futureN != -1) {
            input->s = texs.at(currentTexture)
                           ->fxs.at(input->futureN)
                           ->outputs.at(input->futureS);
          }
        }
      }
      if (currentTexture >= texs.size()) {
        currentTexture = texs.size() - 1;
      }
      if (selectedColor >= palette.size()) {
        selectedColor = palette.size() - 1;
      }
      Color* rgb = palette.at(selectedColor);
      Color hsv = RGBtoHSV(rgb->r, rgb->g, rgb->b);
      colorParams.at(0)->value = hsv.r;
      colorParams.at(0)->value2 = hsv.g / 100.0;
      colorParams.at(0)->value3 = hsv.b / 100.0;
      colorParams.at(1)->value = rgb->r;
      colorParams.at(2)->value = rgb->g;
      colorParams.at(3)->value = rgb->b;
      colorParams.at(4)->value = hsv.r;
      colorParams.at(5)->value = hsv.g;
      colorParams.at(6)->value = hsv.b;
      paletteChanged();
      glGenTextures(1, &screenTexFinal2);
      glBindTexture(GL_TEXTURE_2D, screenTexFinal2);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSizeX, texSizeY, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glGenFramebuffers(1, &screenFboFinal2);
      glBindFramebuffer(GL_FRAMEBUFFER, screenFboFinal2);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_2D, screenTexFinal2, 0);
      GLenum d2[] = {GL_COLOR_ATTACHMENT0};
      glDrawBuffers(1, d2);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      for (int i = 0; i < texs.size(); i++) {
        Texture* t = texs.at(i);
        t->genTex();
      }
    } else {
      if (errorMessage.size() > 0) {
        errorMessage += "\n";
      }
      errorMessage += "Invalid file";
      errorTimer = 160;
      error = true;
    }
    fin.clear();
    if (error) {
      ::newFile();
    }
  }
}

void newFile() {
  listUndo.clear();
  listRedo.clear();
  nodeCX = 0;
  nodeCY = 0;
  nodeCSX = 0;
  nodeCSY = 0;
  texSizeX = 32;
  texSizeY = texSizeX;
  glGenTextures(1, &screenTexFinal2);
  glBindTexture(GL_TEXTURE_2D, screenTexFinal2);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSizeX, texSizeY, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenFramebuffers(1, &screenFboFinal2);
  glBindFramebuffer(GL_FRAMEBUFFER, screenFboFinal2);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         screenTexFinal2, 0);
  GLenum d2[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, d2);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  for (int toDel = 0; toDel < texs.size(); toDel++) {
    delete texs.at(toDel);
  }
  texs.clear();
  Texture* t = new Texture();
  texs.push_back(t);

  currentTexture = 0;
  for (int toDel = 0; toDel < palette.size(); toDel++) {
    delete palette.at(toDel);
  }
  palette.clear();
  palette.push_back(new Color(0, 0, 0));
  paletteChanged();
  selectedColor = 0;
  zoom = 1;
  for (int i = 0; i < colorParams.size(); i++) {
    colorParams.at(i)->value = colorParams.at(i)->resetValue;
  }
  colorParams.at(0)->value = 0;
  colorParams.at(0)->value2 = 0;
  colorParams.at(0)->value3 = 0;
  lastPalDir = "";
  lastPalName = "";
  lastTexDir = "";
  lastTexName = "";
  lastSaveDir = "";
  lastSaveName = "";
}

void loadFile(std::string dir) {
  std::ifstream fin(dir);
  std::stringstream buffer;
  buffer << fin.rdbuf();
  loadStuff(buffer.str(), true);
}

void saveFile(std::string dir) {
  std::ofstream out(dir.c_str());
  out << saveStuff();
}
