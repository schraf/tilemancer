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

#ifndef TILEMANCER_GRAPHICS_GLOBALS_H
#define TILEMANCER_GRAPHICS_GLOBALS_H

#include "glm/glm.hpp"
#include "tilemancer/gl.h"

extern GLuint gridImg;
extern GLuint effectImg;
extern GLuint effectImg2;
extern GLuint effectImg3;
extern GLuint effectImg4;
extern GLuint effectImg5;
extern GLuint effectImg6;
extern GLuint effectImg8;
extern GLuint effectImg10;
extern GLuint effectImg11;
extern GLuint effectImg14;
extern GLuint effectImg15;
extern GLuint effectImg16;
extern GLuint bezierFill;
extern GLuint bezierFillError;
extern GLuint iconImg2;
extern GLuint iconImg3;
extern GLuint iconImg6;
extern GLuint iconImg7;
extern GLuint iconImg8;
extern GLuint iconImg9;
extern GLuint iconImg10;
extern GLuint iconImg12;
extern GLuint iconImg13;
extern GLuint palImg;
extern GLuint palImgReal;
extern GLuint fontImg;
extern GLuint fontImg2;

extern GLenum my_program;
extern GLenum my_vertex_shader;
extern GLenum my_fragment_shader;

extern GLenum light_program;
extern GLenum light_vertex_shader;
extern GLenum light_fragment_shader;

extern GLenum blur_program;
extern GLenum blur_vertex_shader;
extern GLenum blur_fragment_shader;

extern GLenum transition_program;
extern GLenum transition_vertex_shader;
extern GLenum transition_fragment_shader;

extern GLuint screenFbo;
extern GLuint screenTex;
extern GLuint screenTexN;
extern GLuint screenTexB;
extern GLuint screenTexMisc;

extern GLuint screenFboFinal2;
extern GLuint screenTexFinal2;

extern GLuint vbo;
extern GLuint vboTri;
extern GLuint vao;

extern GLuint digitsImg;
extern GLuint noTex;
extern GLuint logoImage;

extern GLint mp_tex;
extern GLint mp_texN;
extern GLint mp_frameSize;
extern GLint mp_texSize;
extern GLint mp_off;
extern GLint mp_frame;
extern GLint mp_depth;
extern GLint mp_alpha;
extern GLint mp_strength;
extern GLint mp_model;
extern GLint mp_flip;
extern GLint mp_proj;

extern glm::mat4 model;
extern glm::mat4 proj;

#endif  // TILEMANCER_GRAPHICS_GLOBALS_H
