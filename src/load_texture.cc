<<<<<<< HEAD:src/load_texture.cc
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

#include "tilemancer/load_texture.h"

#include <iostream>

#ifdef _WIN32
GLuint loadTexture(const std::string& path) {
  GLuint tex;
  unzFile data = unzOpen("data.lpf");
  unz_file_info info;
  Uint8* buffer;
  SDL_RWops* rw = NULL;
  SDL_Surface* ls = NULL;

  unzLocateFile(data, path.c_str(), NULL);
  unzOpenCurrentFile(data);
  unzGetCurrentFileInfo(data, &info, NULL, 0, NULL, 0, NULL, 0);
  buffer = (Uint8*)malloc(info.uncompressed_size);
  unzReadCurrentFile(data, buffer, info.uncompressed_size);
  rw = SDL_RWFromConstMem(buffer, info.uncompressed_size);
  ls = IMG_LoadPNG_RW(rw);
  free(buffer);

  if (ls != NULL) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    int mode = GL_RGB;
    int mode2 = GL_BGR;
    if (ls->format->BytesPerPixel == 4) {
      mode = GL_RGBA;
      mode2 = GL_BGRA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, ls->w, ls->h, 0, mode,
                 GL_UNSIGNED_BYTE, ls->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // SDL_FreeSurface(ls);
  } else {
    // cout << "Failed to load image: "+path << endl;
  }
  return tex;
}

GLuint loadTexture2(const std::string& path) {
  GLuint tex;
  SDL_Surface* ls = IMG_Load(path.c_str());
  if (ls != NULL) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    int mode = GL_RGB;
    int mode2 = GL_BGR;
    if (ls->format->BytesPerPixel == 4) {
      mode = GL_RGBA;
      mode2 = GL_BGRA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, ls->w, ls->h, 0, mode,
                 GL_UNSIGNED_BYTE, ls->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // SDL_FreeSurface(ls);
  } else {
    // cout << "Failed to load image: "+path << endl;
  }
  return tex;
}
#elif defined(__APPLE__) || defined(__linux__)
GLuint loadTexture(const std::string& path) {
  GLuint tex;
  SDL_Surface* ls = IMG_Load(path.c_str());
  if (ls != NULL) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    int mode = GL_RGB;
    int mode2 = GL_BGR;
    if (ls->format->BytesPerPixel == 4) {
      mode = GL_RGBA;
      mode2 = GL_BGRA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, ls->w, ls->h, 0, mode2,
                 GL_UNSIGNED_BYTE, ls->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // SDL_FreeSurface(ls);
  } else {
    std::cout << "Failed to load image: " + path << std::endl;
  }
  return tex;
}

GLuint loadTexture2(const std::string& path) {
  std::cout << "Loading texture2 " << path << std::endl;
  return 0;
}
#else
=======
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

#include "tilemancer/load_texture.h"
#include <iostream>
#include "tilemancer/image.h"

#ifdef _WIN32
GLuint loadTexture(const std::string& path) {
  GLuint tex;
  unzFile data = unzOpen("data.lpf");
  unz_file_info info;
  Uint8* buffer;
  SDL_RWops* rw = NULL;
  SDL_Surface* ls = NULL;

  unzLocateFile(data, path.c_str(), NULL);
  unzOpenCurrentFile(data);
  unzGetCurrentFileInfo(data, &info, NULL, 0, NULL, 0, NULL, 0);
  buffer = (Uint8*)malloc(info.uncompressed_size);
  unzReadCurrentFile(data, buffer, info.uncompressed_size);
  rw = SDL_RWFromConstMem(buffer, info.uncompressed_size);
  ls = IMG_LoadPNG_RW(rw);
  free(buffer);

  if (ls != NULL) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    int mode = GL_RGB;
    int mode2 = GL_BGR;
    if (ls->format->BytesPerPixel == 4) {
      mode = GL_RGBA;
      mode2 = GL_BGRA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, ls->w, ls->h, 0, mode,
                 GL_UNSIGNED_BYTE, ls->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // SDL_FreeSurface(ls);
  } else {
    // cout << "Failed to load image: "+path << endl;
  }
  return tex;
}

GLuint loadTexture2(const std::string& path) {
  GLuint tex;
  SDL_Surface* ls = IMG_Load(path.c_str());
  if (ls != NULL) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    int mode = GL_RGB;
    int mode2 = GL_BGR;
    if (ls->format->BytesPerPixel == 4) {
      mode = GL_RGBA;
      mode2 = GL_BGRA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, ls->w, ls->h, 0, mode,
                 GL_UNSIGNED_BYTE, ls->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // SDL_FreeSurface(ls);
  } else {
    // cout << "Failed to load image: "+path << endl;
  }
  return tex;
}
#elif defined(__APPLE__) || defined(__linux__)
GLuint loadTexture(const std::string& path) {
  GLuint tex;
  ImageLoadResult ls = LoadImage(path, AlphaLoad::Include);
  if (ls.height <= 0 ){
    std::cout << "Failed to load image " + path << ": " << ls.error << std::endl;
    return 0;
  }

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  const int mode = ls.has_alpha ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, mode, ls.width, ls.height, 0, mode,
               GL_UNSIGNED_BYTE, &ls.components[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return tex;
}

GLuint loadTexture2(const std::string& path) {
  std::cout << "Loading texture2 " << path << std::endl;
  return 0;
}
#else
>>>>>>> f1c3825... replaced sdl image loading with stb:tilemancer/load_texture.cc
#endif