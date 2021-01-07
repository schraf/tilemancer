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

#include "tilemancer/graphics_globals.h"

GLuint gridImg;
GLuint effectImg;
GLuint effectImg2;
GLuint effectImg3;
GLuint effectImg4;
GLuint effectImg5;
GLuint effectImg6;
GLuint effectImg8;
GLuint effectImg10;
GLuint effectImg11;
GLuint effectImg14;
GLuint effectImg15;
GLuint effectImg16;
GLuint bezierFill;
GLuint bezierFillError;
GLuint iconImg2;
GLuint iconImg3;
GLuint iconImg6;
GLuint iconImg7;
GLuint iconImg8;
GLuint iconImg9;
GLuint iconImg10;
GLuint iconImg12;
GLuint iconImg13;
GLuint palImg;
GLuint palImgReal;
GLuint fontImg;
GLuint fontImg2;

GLenum my_program;
GLenum my_vertex_shader;
GLenum my_fragment_shader;

GLenum light_program;
GLenum light_vertex_shader;
GLenum light_fragment_shader;

GLenum blur_program;
GLenum blur_vertex_shader;
GLenum blur_fragment_shader;

GLenum transition_program;
GLenum transition_vertex_shader;
GLenum transition_fragment_shader;

GLuint screenFbo;
GLuint screenTex;
GLuint screenTexN;
GLuint screenTexB;
GLuint screenTexMisc;

GLuint screenFboFinal2;
GLuint screenTexFinal2;

GLuint vbo;
GLuint vboTri;
GLuint vao;

GLuint digitsImg;
GLuint noTex;
GLuint logoImage;

GLint mp_tex;
GLint mp_texN;
GLint mp_frameSize;
GLint mp_texSize;
GLint mp_off;
GLint mp_frame;
GLint mp_depth;
GLint mp_alpha;
GLint mp_strength;
GLint mp_model;
GLint mp_flip;
GLint mp_proj;

glm::mat4 model = glm::mat4(1.0);
glm::mat4 proj = glm::mat4(1.0);
