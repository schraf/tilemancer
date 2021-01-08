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

#include "tilemancer/browser.h"

#include "tilemancer/globals.h"
#include "tilemancer/graphics_globals.h"
#include "tilemancer/browserfile.h"
#include "tilemancer/socket.h"
#include "tilemancer/saveload.h"
#include "tilemancer/palette.h"
#include "tilemancer/render.h"
#include "tilemancer/load_texture.h"

#include "tinydir.h"

#include <vector>

namespace {
  BrowserMode browserMode = BrowserMode::e5Save;
  float browserScroll;
  bool browserOpen;
  std::vector<BrowserFile*> filenames;

  std::vector<std::string> fnUndo;
  std::vector<std::string> fnRedo;

  int selectedFile;

  bool overwrite;
}

void browserOnBackspace() {
  if (filenameB.size() > 0) {
    filenameB.pop_back();
  }
  overwrite = false;
}

void browserAction(std::string dir, std::string subDir, std::string parent) {
  bool exists = false;
  for (int i = 0; i < filenames.size(); i++) {
    if (!strcmp(filenames.at(i)->name.c_str(), subDir.c_str())) {
      exists = true;
    }
  }
  if (browserMode == BrowserMode::e0Import) {
    if (exists) {
      palImg = loadTexture(dir);
      loadPalette();
      browserOpen = false;
      fnUndo.clear();
      fnRedo.clear();
    }
  } else if (browserMode == BrowserMode::e1Export) {
    if (!exists || overwrite) {
      exportPalette(dir);
      lastPalName = subDir;
      lastPalDir = parent;
      browserOpen = false;
      fnUndo.clear();
      fnRedo.clear();
    } else {
      overwrite = true;
    }
  } else if (browserMode == BrowserMode::e2ExportTex) {
    if (!exists || overwrite) {
      exportTexSingle(dir);
      currentSocket->lastTexName = subDir;
      currentSocket->lastTexDir = parent;
      browserOpen = false;
      fnUndo.clear();
      fnRedo.clear();
    } else {
      overwrite = true;
    }
  } else if (browserMode == BrowserMode::e3) {
    if (!exists || overwrite) {
      // exportTexMulti(dir);
      lastTexName = subDir;
      lastTexDir = parent;
      browserOpen = false;
      fnUndo.clear();
      fnRedo.clear();
    } else {
      overwrite = true;
    }
  } else if (browserMode == BrowserMode::e4Open) {
    if (exists) {
      listUndo.clear();
      listRedo.clear();
      loadFile(dir);
      browserOpen = false;
      fnUndo.clear();
      fnRedo.clear();
    }
  } else if (browserMode == BrowserMode::e5Save) {
    if (!exists || overwrite) {
      lastSaveName = subDir;
      lastSaveDir = parent;
      saveFile(dir);
      browserOpen = false;
      fnUndo.clear();
      fnRedo.clear();
    } else {
      overwrite = true;
    }
  }
}

#ifdef _WIN32
void openBrowser(std::string dir, int type, BrowserMode mode) {
  overwrite = false;
  browserMode = mode;
  browserScroll = 0;
  if (type == 1) {  // undo
    fnUndo.pop_back();
    fnRedo.push_back(currentDir);
  } else if (type == 2) {  // redo
    fnRedo.pop_back();
    fnUndo.push_back(currentDir);
  } else {
    fnUndo.push_back(currentDir);
    fnRedo.clear();
  }
  selectedFile = -1;
  currentDir = dir;
  filenameB = "";
  if (browserMode == BrowserMode::e4Open && lastPalDir.size() > 0 && !browserOpen) {
    currentDir = lastPalDir;
    filenameB = lastPalName;
  }
  Texture* t = texs.at(currentTexture);
  if (browserMode == BrowserMode::e2ExportTex && currentSocket->lastTexDir.size() > 0 &&
      !browserOpen) {
    currentDir = currentSocket->lastTexDir;
    filenameB = currentSocket->lastTexName;
  }
  if (browserMode == BrowserMode::e0Import && lastTexDir.size() > 0 && !browserOpen) {
    currentDir = lastTexDir;
    filenameB = lastTexName;
  }
  if (browserMode == BrowserMode::e3 && lastSaveDir.size() > 0 && !browserOpen) {
    currentDir = lastSaveDir;
    filenameB = lastSaveName;
  }
  for (int toDel = 0; toDel < filenames.size(); toDel++) {
    delete filenames.at(toDel);
  }
  filenames.clear();

  tinydir_dir dirr;
  std::vector<BrowserFile*> temp;

  if (tinydir_open(&dirr, currentDir.c_str()) == 0) {
    while (dirr.has_next) {
      tinydir_file file;
      tinydir_readfile(&dirr, &file);

      if (file.name[0] != '.') {
        std::string fullDir = currentDir;
        fullDir = fullDir.append("\\");
        fullDir = fullDir.append(std::string(file.name));
        BrowserFile* a = new BrowserFile(std::string(file.name), file.is_dir);
        if (a->folder) {
          filenames.push_back(a);
        } else {
          temp.push_back(a);
        }
      }

      tinydir_next(&dirr);
    }
    tinydir_close(&dirr);
  }

  filenames.insert(filenames.end(), temp.begin(), temp.end());
  browserOpen = true;
}

void getHome() {
  const char* homeDir;
  homeDir = getenv("HOMEPATH");
  currentDir = homeDir;
}

#elif defined(__APPLE__) || defined(__linux__)
void openBrowser(std::string dir, int type, BrowserMode mode) {
  overwrite = false;
  browserMode = mode;
  browserScroll = 0;
  if (type == 1) {  // undo
    fnUndo.pop_back();
    fnRedo.push_back(currentDir);
  } else if (type == 2) {  // redo
    fnRedo.pop_back();
    fnUndo.push_back(currentDir);
  } else {
    fnUndo.push_back(currentDir);
    fnRedo.clear();
  }
  selectedFile = -1;
  currentDir = dir;
  filenameB = "";
  if (browserMode == BrowserMode::e1Export && lastPalDir.size() > 0 && !browserOpen) {
    currentDir = lastPalDir;
    filenameB = lastPalName;
  }
  Texture* t = texs.at(currentTexture);
  if (browserMode == BrowserMode::e2ExportTex && currentSocket->lastTexDir.size() > 0 &&
      !browserOpen) {
    currentDir = currentSocket->lastTexDir;
    filenameB = currentSocket->lastTexName;
  }
  if (browserMode == BrowserMode::e3 && lastTexDir.size() > 0 && !browserOpen) {
    currentDir = lastTexDir;
    filenameB = lastTexName;
  }
  if (browserMode == BrowserMode::e5Save && lastSaveDir.size() > 0 && !browserOpen) {
    currentDir = lastSaveDir;
    filenameB = lastSaveName;
  }
  for (int toDel = 0; toDel < filenames.size(); toDel++) {
    delete filenames.at(toDel);
  }
  filenames.clear();
  int num_entries;
  struct dirent** entries = NULL;
  num_entries = scandir(currentDir.c_str(), &entries, NULL, NULL);
  std::vector<BrowserFile*> temp;
  for (int i = 0; i < num_entries; i++) {
    if (entries[i]->d_name[0] != '.') {
      std::string fullDir = currentDir;
      fullDir = fullDir.append("/");
      fullDir = fullDir.append(std::string(entries[i]->d_name));
      struct stat path_stat;
      stat(fullDir.c_str(), &path_stat);
      BrowserFile* a =
          new BrowserFile(std::string(entries[i]->d_name), !S_ISREG(path_stat.st_mode));
      if (a->folder) {
        filenames.push_back(a);
      } else {
        temp.push_back(a);
      }
    }
  }
  filenames.insert(filenames.end(), temp.begin(), temp.end());
  for (int i = 0; i < num_entries; i++) {
    free(entries[i]);
  }
  free(entries);
  browserOpen = true;
}

void getHome() {
  char* homeDir;
  homeDir = getenv("HOME");
  if (!homeDir) {
    struct passwd* pwd = getpwuid(getuid());
    if (pwd) {
      homeDir = pwd->pw_dir;
    }
  }
  currentDir = homeDir;
}

#else
#endif

void onBrowserResize() {
  adjustBrowserScroll(0.0f);
}

bool browserIsOpen() {
  return browserOpen;
}

void adjustBrowserScroll(float adjust) {
  browserScroll += adjust;
  int title = collH - 4;
  int bSpace = 5;
  float tS = title / 2 - 4;
  int fh = tS;
  int fw = -(screenW - barXRight - barX - bSpace * 2);
  int fwNext = 0;
  for (int i = 0; i < filenames.size(); i++) {
    int n = textW(filenames.at(i)->name, barX + 1 + bSpace + fw + tS + 8,
                  title * 2 + bSpace * 3 + fh, fontImg, 0);
    if (n > fwNext) {
      fwNext = n;
    }
    fh += title;
    if (fh + title * 2 + title + bSpace * 3 >
            title + bSpace * 2 + barY - 1 - bSpace * 3 - title - 6 &&
        i != filenames.size() - 1) {
      fh = tS;
      fw += fwNext + tS * 2 + 8 * 3;
      fwNext = 0;
    }
  }
  fw += fwNext + tS * 2 + 8 * 3;
  if (browserScroll > fw) {
    browserScroll = fw;
  }
  if (browserScroll < 0) {
    browserScroll = 0;
  }
}

void renderFileBrowser(int barX, int barY, int barXRight, int scrollW) {
  renderUI(barX + 1, 0, screenW - barX - barXRight - 2, barY - 1, effectImg5);
  int title = collH - 4;
  int bSpace = 5;
  float tS = title / 2 - 4;
  renderUI(barX + 1 + bSpace * 4 + 8 * 3, bSpace,
             screenW - barX - barXRight - 2 - bSpace * 6 - 8 * 4, title,
             effectImg6);
  int ln = textW(currentDir, barX + 1 + bSpace * 4 + 8 * 3 + tS, bSpace + tS,
                   fontImg, 0);
  if (ln > screenW - barX - barXRight - 2 - bSpace * 6 - 8 * 4 - tS * 2) {
      renderText(currentDir, screenW - barXRight - 1 - bSpace * 2 - 8 - tS,
                 bSpace + tS, fontImg, 1);
    } else {
      renderText(currentDir, barX + 1 + bSpace * 4 + 8 * 3 + tS, bSpace + tS,
                 fontImg, 0);
    }
  renderUI(barX + 1 + bSpace, title + bSpace * 2,
             screenW - barX - barXRight - 2 - bSpace * 3 - 8, title,
             effectImg6);
  if (overwrite) {
      renderText(filenameB, barX + 1 + bSpace + tS, bSpace * 2 + tS + title,
                 fontImg2, 0);
    } else {
      renderText(filenameB, barX + 1 + bSpace + tS, bSpace * 2 + tS + title,
                 fontImg, 0);
    }
  renderUI(barX + 1 + bSpace, title * 2 + bSpace * 3,
             screenW - barX - barXRight - 2 - bSpace * 2,
             barY - 1 - bSpace * 4 - title * 2, effectImg6);
  int fh = tS;
  int fw = tS - browserScroll;
  int fwNext = 0;
  for (int i = 0; i < filenames.size(); i++) {
      int n = textW(filenames.at(i)->name, barX + 1 + bSpace + fw + tS + 8,
                    title * 2 + bSpace * 3 + fh, fontImg, 0);
      if (barX + 1 + bSpace + fw - tS + n + 8 * 2 + tS > barX &&
          barX + 1 + bSpace + fw - tS < screenW - barXRight) {
        if (i == selectedFile) {
          renderUI(barX + 1 + bSpace + fw - tS,
                   title * 2 + bSpace * 3 + fh - tS, n + 8 * 2 + tS, title,
                   effectImg5);
        }
        renderIcon(barX + 1 + bSpace + fw, title * 2 + bSpace * 3 + fh, 8, 8,
                   iconImg9, filenames.at(i)->folder);
        renderText(filenames.at(i)->name, barX + 1 + bSpace + fw + tS + 8,
                   title * 2 + bSpace * 3 + fh, fontImg, 0);
      }

      if (n > fwNext) {
        fwNext = n;
      }
      fh += title;
      if (fh + title * 2 + title + bSpace * 3 >
              title + bSpace * 2 + barY - 1 - bSpace * 3 - title - 6 &&
          i != filenames.size() - 1) {
        fh = tS;
        fw += fwNext + tS * 2 + 8 * 3;
        fwNext = 0;
      }
    }
  fw += fwNext + tS * 2 + 8 * 3;
  int scrollH = fw + browserScroll;
  if (scrollH < screenW - barX - barXRight - bSpace * 2) {
      scrollH = screenW - barX - barXRight - bSpace * 2;
    }
  renderUI(barX + bSpace + 1 + int(int(browserScroll) *
                                     (screenW - barX - barXRight - bSpace * 2) /
                                     float(scrollH)),
             barY - bSpace - scrollW - 1,
             int((screenW - barX - barXRight - bSpace * 2) *
                 (screenW - barX - barXRight - bSpace * 2) / float(scrollH)),
             scrollW, effectImg14);

  renderUI(barX, 0, bSpace, barY - 1, effectImg);
  renderUI(screenW - barXRight - bSpace, 0, bSpace, barY - 1, effectImg);
  renderUI(barX + 1, 0, bSpace, barY - 1, effectImg5);
  renderUI(barX + 1, 0, bSpace * 4 + 8 * 3, title + bSpace, effectImg5);
  renderUI(screenW - barXRight - bSpace - 1, 0, bSpace, barY - 1, effectImg5);
  renderUI(screenW - barXRight - bSpace * 2 - 1 - 8, 0, bSpace * 2 + 8,
             title * 2 + bSpace * 2, effectImg5);
  bool leftActive = (fnUndo.size() > 1);
  bool rightActive = (fnRedo.size() > 0);
  bool upActive = (currentDir.size() != 1);
  renderIcon(barX + 1 + bSpace, bSpace + tS, 8, 8, iconImg10, 0 + leftActive);
  renderIcon(barX + 1 + bSpace + 8 + bSpace, bSpace + tS, 8, 8, iconImg10,
               2 + rightActive);
  renderIcon(barX + 1 + bSpace + 8 * 2 + bSpace * 2, bSpace + tS, 8, 8,
               iconImg10, 4 + upActive);
  renderIcon(screenW - barXRight - 8 - bSpace - 1, bSpace + tS, 8, 8,
               iconImg10, 6);
  int impexp = 0;
  if (browserMode == BrowserMode::e1Export || browserMode == BrowserMode::e2ExportTex) {
      impexp = 1;
    } else if (browserMode == BrowserMode::e3) {
      impexp = 2;
    } else if (browserMode == BrowserMode::e4Open) {
      impexp = 4;
    } else if (browserMode == BrowserMode::e5Save) {
      impexp = 3;
    }
  renderIcon(screenW - barXRight - 8 - bSpace - 1, bSpace * 2 + title + tS, 8,
               8, iconImg7, impexp);
}

void browserOnEscape() {
  browserOpen = false;
  fnUndo.clear();
  fnRedo.clear();
}

void browserOnEnter() {
  std::string fullDir = currentDir;
  if (fullDir.size() != 1) {
        fullDir = fullDir.append("/");
      }
  fullDir = fullDir.append(filenameB);
  bool folder = false;
  for (int i = 0; i < filenames.size(); i++) {
        if (!strcmp(filenames.at(i)->name.c_str(), filenameB.c_str()) &&
            filenames.at(i)->folder) {
          openBrowser(fullDir, 0, browserMode);
          folder = true;
        }
      }
  if (!folder) {
        browserAction(fullDir, filenameB, currentDir);
      }
}

void browserOnText(const std::string &text) {
  filenameB.append(text);
  overwrite = false;
}

void browserButtonDown(int x, int y) {
  int title = collH - 4;
  int bSpace = 5;
  float tS = title / 2 - 4;
  if (x > barX + bSpace && x < screenW - barXRight - bSpace) {
          int fh = tS;
          int fw = tS - browserScroll;
          int fwNext = 0;
          for (int i = 0; i < filenames.size(); i++) {
            int n =
                textW(filenames.at(i)->name, barX + 1 + bSpace + fw + tS + 8,
                      title * 2 + bSpace * 3 + fh, fontImg, 0);
            if (x > barX + 1 + bSpace + fw - tS &&
                x < barX + 1 + bSpace + fw - tS + n + 8 * 2 + tS &&
                y > title * 2 + bSpace * 3 + fh - tS &&
                y < title * 2 + bSpace * 3 + fh - tS + title) {
              if (doubleClickTimer <= 20 && selectedFile == i) {
                std::string fullDir = currentDir;
                if (fullDir.size() != 1) {
                  fullDir = fullDir.append("/");
                }
                fullDir = fullDir.append(std::string(filenames.at(i)->name));
                if (filenames.at(i)->folder) {
                  openBrowser(fullDir, 0, browserMode);
                } else {
                  browserAction(fullDir, filenames.at(i)->name, currentDir);
                }
              } else {
                if (selectedFile != i) {
                  overwrite = false;
                }
                selectedFile = i;
                filenameB = filenames.at(i)->name;
              }
              doubleClickTimer = 0;
            }
            if (n > fwNext) {
              fwNext = n;
            }
            fh += title;
            if (fh + title * 2 + title + bSpace * 3 >
                    title + bSpace * 2 + barY - 1 - bSpace * 3 - title - 6 &&
                i != filenames.size() - 1) {
              fh = tS;
              fw += fwNext + tS * 2 + 8 * 3;
              fwNext = 0;
            }
          }
          fw += fwNext + tS * 2 + 8 * 3;
          int scrollH = fw + browserScroll;
          if (scrollH < screenW - barX - barXRight - bSpace * 2) {
            scrollH = screenW - barX - barXRight - bSpace * 2;
          }
          if (x > barX + bSpace + 1 +
                      int(int(browserScroll) *
                          (screenW - barX - barXRight - bSpace * 2) /
                          float(scrollH)) &&
              x < barX + bSpace + 1 +
                      int(int(browserScroll) *
                          (screenW - barX - barXRight - bSpace * 2) /
                          float(scrollH)) +
                      int((screenW - barX - barXRight - bSpace * 2) *
                          (screenW - barX - barXRight - bSpace * 2) /
                          float(scrollH)) &&
              y > barY - bSpace - 6 - 1 && y < barY - bSpace - 1) {
            draggingSBar = true;
            mouseOX = x;
            mouseOY = y;
            mouseX = x;
            mouseY = y;
            sBarDrag = 6;
            sRatio = scrollH / float(screenW - barX - barXRight - bSpace * 2);
          }
        }
  if (x > barX + 1 + bSpace && x < barX + 1 + bSpace + 8 &&
            y > bSpace + tS && y < bSpace + tS + 8) {
          // left
          if (fnUndo.size() > 1) {
            openBrowser(fnUndo.back(), 1, browserMode);
          }
        }
  if (x > barX + 1 + bSpace + 8 + bSpace &&
            x < barX + 1 + bSpace + 8 + 8 + bSpace && y > bSpace + tS &&
            y < bSpace + tS + 8) {
          // right
          if (fnRedo.size() > 0) {
            openBrowser(fnRedo.back(), 2, browserMode);
          }
        }
  if (x > barX + 1 + bSpace + 8 * 2 + bSpace * 2 &&
            x < barX + 1 + bSpace + 8 + 8 * 2 + bSpace * 2 && y > bSpace + tS &&
            y < bSpace + tS + 8) {
          // up
          std::string newDir = currentDir;
          newDir.erase(newDir.rfind('/'));
          if (newDir.size() < 1) {
            newDir = "/";
          }
          openBrowser(newDir, 0, browserMode);
        }
  if (x > screenW - barXRight - 8 - bSpace - 1 &&
            x < screenW - barXRight - bSpace - 1 && y > bSpace + tS &&
            y < bSpace + tS + 8) {
          browserOpen = false;
          fnUndo.clear();
          fnRedo.clear();
        }
  if (x > screenW - barXRight - 8 - bSpace - 1 &&
            x < screenW - barXRight - bSpace - 1 &&
            y > bSpace * 2 + title + tS && y < bSpace * 2 + title + tS + 8) {
          // action
          std::string fullDir = currentDir;
          if (fullDir.size() != 1) {
            fullDir = fullDir.append("/");
          }
          fullDir = fullDir.append(filenameB);
          bool folder = false;
          for (int i = 0; i < filenames.size(); i++) {
            if (!strcmp(filenames.at(i)->name.c_str(), filenameB.c_str()) &&
                filenames.at(i)->folder) {
              openBrowser(fullDir, 0, browserMode);
              folder = true;
            }
          }
          if (!folder) {
            browserAction(fullDir, filenameB, currentDir);
          }
        }
}
