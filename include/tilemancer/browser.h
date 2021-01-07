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

#ifndef TILEMANCER_BROWSER_H
#define TILEMANCER_BROWSER_H

#include <string>

// e3 is used by code, but never used in calls, remove?
enum class BrowserMode {
  e5Save, e4Open, e2ExportTex, e0Import, e1Export, e3
};

void browserOnBackspace();
bool browserIsOpen();
void browserOnEnter();
void browserOnEscape();
void browserOnText(const std::string &text);
void browserButtonDown(int x, int y);
void browserAction(std::string dir, std::string subDir, std::string parent);
void openBrowser(std::string dir, int type, BrowserMode mode);
void adjustBrowserScroll(float adjust) ;
void onBrowserResize();
void renderFileBrowser(int barX, int barY, int barXRight, int scrollW);

#endif  // TILEMANCER_BROWSER_H