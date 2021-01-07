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

#include "tilemancer/render.h"
#include "tilemancer/bezier.h"
#include "tilemancer/color.h"
#include "tilemancer/globals.h"
#include "tilemancer/graphics_globals.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

void renderGradient(int x, int y, int w, int h, Color* c, Color* c2) {
  glm::mat4 currentMatrix = model;
  currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, 0.0));
  currentMatrix = glm::scale(currentMatrix, glm::vec3(w, h, 1.0));
  glUseProgram(light_program);
  glUniform2f(glGetUniformLocation(light_program, "frameSize"), (float)w,
              (float)h);
  glUniform2f(glGetUniformLocation(light_program, "texSize"), (float)w,
              (float)h);
  glUniform1i(glGetUniformLocation(light_program, "frame"), 0);
  glUniform3f(glGetUniformLocation(light_program, "color"), (float)c->r / 255.0,
              (float)c->g / 255.0, (float)c->b / 255.0);
  glUniform3f(glGetUniformLocation(light_program, "color2"),
              (float)c2->r / 255.0, (float)c2->g / 255.0, (float)c2->b / 255.0);
  glUniformMatrix4fv(glGetUniformLocation(light_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(currentMatrix));
  glUniformMatrix4fv(glGetUniformLocation(light_program, "proj"), 1, GL_FALSE,
                     glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(glGetAttribLocation(light_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(light_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(light_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(light_program, "t"));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glUseProgram(0);
}

void renderColor(int x, int y, int w, int h, Color* c, int cutoff, int cutoff2,
                 int cutoff3, int cutoff4) {
  if (x < cutoff && cutoff != -1) {
    w = x + w - cutoff;
    if (w < 0) {
      w = 0;
    }
    x = cutoff;
  }
  if (x + w > cutoff2 && cutoff2 != -1) {
    w = cutoff2 - x;
    if (w < 0) {
      w = 0;
    }
  }
  if (y < cutoff3 && cutoff3 != -1) {
    h = y + h - cutoff3;
    if (h < 0) {
      h = 0;
    }
    y = cutoff3;
  }
  if (y + h > cutoff4 && cutoff4 != -1) {
    h = cutoff4 - y;
    if (h < 0) {
      h = 0;
    }
  }
  glm::mat4 currentMatrix = model;
  currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, 0.0));
  currentMatrix = glm::scale(currentMatrix, glm::vec3(w, h, 1.0));
  glUseProgram(light_program);
  glUniform2f(glGetUniformLocation(light_program, "frameSize"), (float)w,
              (float)h);
  glUniform2f(glGetUniformLocation(light_program, "texSize"), (float)w,
              (float)h);
  glUniform1i(glGetUniformLocation(light_program, "frame"), 0);
  glUniform2f(glGetUniformLocation(light_program, "off"), (float)0, (float)0);
  glUniform3f(glGetUniformLocation(light_program, "color"), (float)c->r / 255.0,
              (float)c->g / 255.0, (float)c->b / 255.0);
  glUniform3f(glGetUniformLocation(light_program, "color2"),
              (float)c->r / 255.0, (float)c->g / 255.0, (float)c->b / 255.0);
  glUniformMatrix4fv(glGetUniformLocation(light_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(currentMatrix));
  glUniformMatrix4fv(glGetUniformLocation(light_program, "proj"), 1, GL_FALSE,
                     glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(glGetAttribLocation(light_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(light_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(light_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(light_program, "t"));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glUseProgram(0);
}

void renderBezier(Bezier* b, GLuint tex) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glm::mat4 currentMatrix = model;
  glUseProgram(my_program);
  glUniform1i(mp_tex, 0);
  glUniform1i(mp_texN, 1);
  glUniform2f(mp_frameSize, (float)100, (float)100);
  glUniform2f(mp_texSize, (float)100, (float)100);
  glUniform2f(mp_off, (float)0, (float)0);
  glUniform1i(mp_frame, 0);
  glUniform1f(mp_depth, 0);
  glUniform1f(mp_alpha, 1);
  glUniform1f(mp_strength, 0);
  glUniformMatrix4fv(mp_model, 1, GL_FALSE, glm::value_ptr(currentMatrix));
  glUniform2f(mp_flip, (float)0, (float)0);
  glUniformMatrix4fv(mp_proj, 1, GL_FALSE, glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, b->bVbo);
  glVertexAttribPointer(glGetAttribLocation(my_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(my_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(my_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(my_program, "t"));
  glDrawArrays(GL_TRIANGLES, 0, 6 * b->d);
  glUseProgram(0);
}

void renderGlyph(int frame, float x, float y, float w, float h, GLuint tex,
                 int frameW, int frameH, float rot, float centerX,
                 float centerY, float alpha, GLuint texN, float depth,
                 float strength, bool flipX, bool flipY, int cutoff,
                 int cutoff2, int cutoff3, int cutoff4) {
  float offX = 0;
  float offY = 0;
  int o = cutoff - x;
  int o2 = x + w - cutoff2;
  int o3 = cutoff3 - y;
  int o4 = y + h - cutoff4;
  bool disabled = false;
  if (cutoff != -1) {
    if (o > 0 && o < w) {
      int tx = frame % 5;
      x = cutoff;
      float ratio = w / frameW;
      w = w - o;
      frameW = w / ratio;
      offX += o * (tx + 1) / ratio;
    } else if (o >= w) {
      disabled = true;
    } else {
    }
  }
  if (cutoff2 != -1) {
    if (o2 > 0 && o2 < w) {
      int tx = frame % 5;
      float ratio = w / frameW;
      w = w - o2;
      frameW = w / ratio;
      offX += o2 * (tx) / ratio;
    } else if (o2 >= w) {
      disabled = true;
    } else {
    }
  }

  if (cutoff3 != -1) {
    if (o3 > 0 && o3 < h) {
      int ty = frame / 5;
      y = cutoff3;
      float ratio = h / frameH;
      h = h - o3;
      frameH = h / ratio;
      offY += o3 * (ty + 1) / ratio;
    } else if (o3 >= h) {
      disabled = true;
    } else {
    }
  }
  if (cutoff4 != -1) {
    if (o4 > 0 && o4 < h) {
      int ty = frame / 5;
      float ratio = h / frameH;
      h = h - o4;
      frameH = h / ratio;
      offY += o4 * (ty) / ratio;
    } else if (o4 >= h) {
      disabled = true;
    } else {
    }
  }
  glm::mat4 currentMatrix = model;
  currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, 0.0));
  currentMatrix = glm::scale(currentMatrix, glm::vec3(w, h, 1.0));
  glUniform1i(mp_frame, frame);
  glUniform2f(mp_frameSize, (float)frameW, (float)frameH);
  glUniform2f(mp_off, (float)offX, (float)offY);
  glUniformMatrix4fv(mp_model, 1, GL_FALSE, glm::value_ptr(currentMatrix));
  if (!disabled) {
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}

void renderSprite(int frame, float x, float y, float w, float h, GLuint tex,
                  float frameW, float frameH, float rot, float centerX,
                  float centerY, float alpha, GLuint texN, float depth,
                  float strength, bool flipX, bool flipY, float offX,
                  float offY, int cutoff, int cutoff2, int cutoff3,
                  int cutoff4) {
  int tw, th;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);
  glActiveTexture(GL_TEXTURE1);
  if (texN == 0) {
    glBindTexture(GL_TEXTURE_2D, noTex);
  } else {
    glBindTexture(GL_TEXTURE_2D, texN);
  }
  int o = cutoff - x;
  int o2 = x + w - cutoff2;
  int o3 = cutoff3 - y;
  int o4 = y + h - cutoff4;
  bool disabled = false;
  if (cutoff != -1) {
    if (o > 0 && o < w) {
      int tx = frame % 5;
      x = cutoff;
      float ratio = w / frameW;
      w = w - o;
      frameW = w / ratio;
      offX += o * (tx + 1) / ratio;
    } else if (o >= w) {
      disabled = true;
    } else {
    }
  }
  if (cutoff2 != -1) {
    if (o2 > 0 && o2 < w) {
      int tx = frame % 5;
      float ratio = w / frameW;
      w = w - o2;
      frameW = w / ratio;
      offX += o2 * (tx) / ratio;
    } else if (o2 >= w) {
      disabled = true;
    } else {
    }
  }

  if (cutoff3 != -1) {
    if (o3 > 0 && o3 < h) {
      int ty = frame / 5;
      y = cutoff3;
      float ratio = h / frameH;
      h = h - o3;
      frameH = h / ratio;
      offY += o3 * (ty + 1) / ratio;
    } else if (o3 >= h) {
      disabled = true;
    } else {
    }
  }
  if (cutoff4 != -1) {
    if (o4 > 0 && o4 < h) {
      int ty = frame / 5;
      float ratio = h / frameH;
      h = h - o4;
      frameH = h / ratio;
      offY += o4 * (ty) / ratio;
    } else if (o4 >= h) {
      disabled = true;
    } else {
    }
  }
  glm::mat4 currentMatrix = model;
  currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, 0.0));
  if (rot != 0) {
    currentMatrix =
        glm::translate(currentMatrix, glm::vec3(centerX, centerY, 0.0));
    currentMatrix =
        glm::rotate(currentMatrix, glm::radians(rot), glm::vec3(0, 0, 1));
    currentMatrix =
        glm::translate(currentMatrix, glm::vec3(-centerX, -centerY, 0.0));
  }
  currentMatrix = glm::scale(currentMatrix, glm::vec3(w, h, 1.0));
  glUseProgram(my_program);
  glUniform1i(mp_tex, 0);
  glUniform1i(mp_texN, 1);
  glUniform2f(mp_frameSize, (float)frameW, (float)frameH);
  glUniform2f(mp_texSize, (float)tw, (float)th);
  glUniform2f(mp_off, (float)offX, (float)offY);
  glUniform1i(mp_frame, frame);
  glUniform1f(mp_depth, depth);
  glUniform1f(mp_alpha, alpha);
  glUniform1f(mp_strength, strength);
  glUniformMatrix4fv(mp_model, 1, GL_FALSE, glm::value_ptr(currentMatrix));
  glUniform2f(mp_flip, (float)flipX, (float)flipY);
  glUniformMatrix4fv(mp_proj, 1, GL_FALSE, glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(glGetAttribLocation(my_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(my_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(my_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(my_program, "t"));
  if (!disabled) {
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
  glUseProgram(0);
}

void renderNumber(int number, int x, int y, GLuint tex, bool percent,
                  bool alignRight, int cutoff) {
  int kerning[] = {3, 5, 3, 3, 3, 3, 3, 4, 3, 3, 2, 4};
  bool negative = false;
  if (number < 0) {
    negative = true;
    number *= -1;
  }
  int numDigits = 1;
  if (number > 0) {
    numDigits = log10(number) + 1;
  }
  int f = number;
  int w = -1;
  std::vector<int> digits;
  for (int d = 0; d < numDigits; d++) {
    digits.push_back(f % 10);
    w += 9 - kerning[f % 10];
    f /= 10;
  }
  if (percent) {
    w += 9 - kerning[10];
  }
  int k = 0;
  for (int d = digits.size() - 1; d >= 0; d--) {
    renderSprite(digits.at(d) + 55, x + 9 * (digits.size() - 1 - d) - k - w, y,
                 9, 9, tex, 9, 9, 0, 0, 0, 1, 0, 0, 0.0, false, false, 0, 0,
                 cutoff);
    k += kerning[digits.at(d)];
  }
  if (percent) {
    renderSprite(53, x + 9 * digits.size() - k - w, y, 9, 9, tex, 9, 9, 0, 0, 0,
                 1, 0, 0, 0.0, false, false, 0, 0, cutoff);
  }
  if (negative) {
    renderSprite(54, x + 9 * -1 + kerning[11] - w, y, 9, 9, tex, 9, 9, 0, 0, 0,
                 1, 0, 0, 0.0, false, false, 0, 0, cutoff);
  }
}

int renderText(std::string text, int x, int y, GLuint tex, bool alignRight,
               int cutoff, int cutoff2, int cutoff3, int cutoff4, float alpha) {
  int kerning[] = {2,  2, 2, 2, 3, 3, 2, 2, 6, 3, 3, 3, 1, 3, 2, 2, 2, 2, 3,
                   2,  3, 2, 1, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 5, 4, 3, 5,
                   -1, 2, 2, 2, 2, 2, 3, 3, 2, 2, 1, 3, 2, 2, 2, 2, 6,

                   6,  5, 3, 3, 2, 2, 6, 5, 5, 3, 3, 6, 4, 6, 2, 6, 6, 3, 4,
                   3,  2, 1, 5, 4, 5, 5, 4, 4, 6, 4, 4};
  int kerningD[] = {3, 5, 3, 3, 3, 3, 3, 4, 3, 3, 2, 4};
  int w = -1;
  for (int i = 0; i < text.size(); i++) {
    if (text[i] >= 33 && text[i] <= 47) {
      w += 9 - kerning[55 + text[i] - 33];
    } else if (text[i] >= 48 && text[i] <= 57) {
      w += 9 - kerningD[text[i] - 48];
    } else if (text[i] >= 58 && text[i] <= 64) {
      w += 9 - kerning[70 + text[i] - 58];
    } else if (text[i] >= 91 && text[i] <= 95) {
      w += 9 - kerning[77 + text[i] - 91];
    } else if (text[i] >= 123 && text[i] <= 126) {
      w += 9 - kerning[82 + text[i] - 123];
    } else if (text[i] == 32) {
      w += 9 - kerning[52];
    } else if ((text[i] >= 65 && text[i] <= 90) ||
               (text[i] >= 97 && text[i] <= 122)) {
      int f = text[i] - 'A';
      if (f >= 32) {
        f -= 6;
      }
      w += 9 - kerning[f];
    } else {
      w += 9 - kerning[0];
    }
  }
  int wd = w;
  if (!alignRight) {
    w = 0;
  }
  int k = 0;

  int tw, th;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);
  glUseProgram(my_program);
  glUniform1i(mp_tex, 0);
  glUniform1i(mp_texN, 1);
  glUniform2f(mp_texSize, (float)tw, (float)th);
  glUniform1f(mp_depth, 0.0);
  glUniform1f(mp_strength, 0.0);
  glUniform1f(mp_alpha, alpha);
  glUniform2f(mp_flip, (float)0, (float)0);
  glUniformMatrix4fv(mp_proj, 1, GL_FALSE, glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(glGetAttribLocation(my_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(my_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(my_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(my_program, "t"));

  int posX = 0;
  for (int i = 0; i < text.size(); i++) {
    if (text[i] == '\n') {
      y += 9;
      k = 0;
      posX = 0;
    } else {
      if (text[i] >= 33 && text[i] <= 47) {
        renderGlyph(55 + text[i] - 33 + 10, x + posX - k - w, y, 9, 9, tex, 9,
                    9, 0, 0, 0, 1, 0, 0, 0.0, false, false, cutoff, cutoff2,
                    cutoff3, cutoff4);
        k += kerning[55 + text[i] - 33];
      } else if (text[i] >= 48 && text[i] <= 57) {
        renderGlyph(55 + text[i] - 48, x + posX - k - w, y, 9, 9, tex, 9, 9, 0,
                    0, 0, 1, 0, 0, 0.0, false, false, cutoff, cutoff2, cutoff3,
                    cutoff4);
        k += kerningD[text[i] - 48];
      } else if (text[i] >= 58 && text[i] <= 64) {
        renderGlyph(70 + text[i] - 58 + 10, x + posX - k - w, y, 9, 9, tex, 9,
                    9, 0, 0, 0, 1, 0, 0, 0.0, false, false, cutoff, cutoff2,
                    cutoff3, cutoff4);
        k += kerning[70 + text[i] - 58];
      } else if (text[i] >= 91 && text[i] <= 95) {
        renderGlyph(77 + text[i] - 91 + 10, x + posX - k - w, y, 9, 9, tex, 9,
                    9, 0, 0, 0, 1, 0, 0, 0.0, false, false, cutoff, cutoff2,
                    cutoff3, cutoff4);
        k += kerning[77 + text[i] - 91];
      } else if (text[i] >= 123 && text[i] <= 126) {
        renderGlyph(82 + text[i] - 123 + 10, x + posX - k - w, y, 9, 9, tex, 9,
                    9, 0, 0, 0, 1, 0, 0, 0.0, false, false, cutoff, cutoff2,
                    cutoff3, cutoff4);
        k += kerning[82 + text[i] - 123];
      } else if (text[i] == 32) {
        renderGlyph(52, x + posX - k - w, y, 9, 9, tex, 9, 9, 0, 0, 0, 1, 0, 0,
                    0.0, false, false, cutoff, cutoff2, cutoff3, cutoff4);
        k += kerning[52];
      } else if ((text[i] >= 65 && text[i] <= 90) ||
                 (text[i] >= 97 && text[i] <= 122)) {
        int f = text[i] - 'A';
        if (f >= 32) {
          f -= 6;
        }
        renderGlyph(f, x + posX - k - w, y, 9, 9, tex, 9, 9, 0, 0, 0, 1, 0, 0,
                    0.0, false, false, cutoff, cutoff2, cutoff3, cutoff4);
        k += kerning[f];
      } else {
        // renderGlyph(f, x+9*i-k-w, y, 9, 9, tex, 9, 9, 0, 0, 0, 1, 0, 0, 0.0,
        // false, false);
        k += kerning[0];
      }
      posX += 9;
    }
  }

  glUseProgram(0);

  return wd;
}

void renderRotIcon(int x, int y, int w, int h, GLuint tex, int frame) {
  renderSprite(frame, x, y, w, h, tex, w, h, rotTimer, w / 2.0, h / 2.0, 1, 0,
               0, 0.0, false, false);
}

void renderIcon(int x, int y, int w, int h, GLuint tex, int frame, int offX,
                int offY) {
  renderSprite(frame, x, y, w, h, tex, w, h, 0, 0, 0, 1, 0, 0, 0.0, false,
               false, offX * (frame + 1), offY * (frame + 1));
}

void renderWheel(int x, int y, int w, int h, float v) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, palImgReal);
  glm::mat4 currentMatrix = model;
  currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, 0.0));
  currentMatrix = glm::scale(currentMatrix, glm::vec3(w, h, 1.0));
  glUseProgram(blur_program);
  glUniform2f(glGetUniformLocation(blur_program, "frameSize"), 16, 16);
  glUniform2f(glGetUniformLocation(blur_program, "texSize"), 16, 16);
  glUniform1i(glGetUniformLocation(blur_program, "tex"), 0);
  v /= 50.0;
  if (v > 1.0) {
    v = 1.0;
  } else if (v < 0.0) {
    v = 0.0;
  }
  glUniform1i(glGetUniformLocation(blur_program, "tri"), false);
  glUniform1i(glGetUniformLocation(blur_program, "full"), false);
  glUniformMatrix4fv(glGetUniformLocation(blur_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(currentMatrix));
  glUniformMatrix4fv(glGetUniformLocation(blur_program, "proj"), 1, GL_FALSE,
                     glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(glGetAttribLocation(blur_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(blur_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(blur_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(blur_program, "t"));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glUseProgram(0);
}

void renderWheelTri(int x, int y, int w, int h, float v) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, palImgReal);
  float r = 0.5 * sqrt(3) * 2 * w / 3.0;
  float d = r * 2 / w;
  float a = 0.4;
  x += w / 2.0;
  y += h / 2.0;
  w /= d;
  h /= d;
  w *= a * 2;
  h *= a * 2;
  glm::mat4 currentMatrix = model;
  currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, 0.0));
  currentMatrix =
      glm::rotate(currentMatrix, glm::radians(float(v)), glm::vec3(0, 0, 1));
  currentMatrix = glm::scale(currentMatrix, glm::vec3(w, h, 1.0));
  glUseProgram(blur_program);
  glUniform2f(glGetUniformLocation(blur_program, "frameSize"), 16, 16);
  glUniform2f(glGetUniformLocation(blur_program, "texSize"), 16, 16);
  glUniform1i(glGetUniformLocation(blur_program, "tex"), 0);
  glUniform1i(glGetUniformLocation(blur_program, "tri"), true);
  glUniform1i(glGetUniformLocation(blur_program, "full"), false);
  glUniform1f(glGetUniformLocation(blur_program, "v"), v);
  glUniformMatrix4fv(glGetUniformLocation(blur_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(currentMatrix));
  glUniformMatrix4fv(glGetUniformLocation(blur_program, "proj"), 1, GL_FALSE,
                     glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vboTri);
  glVertexAttribPointer(glGetAttribLocation(blur_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(blur_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(blur_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(blur_program, "t"));
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glUseProgram(0);
}

void renderWheelF(int x, int y, int w, int h, float v) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, palImg);
  glm::mat4 currentMatrix = model;
  currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, 0.0));
  currentMatrix = glm::scale(currentMatrix, glm::vec3(w, h, 1.0));
  glUseProgram(blur_program);
  glUniform2f(glGetUniformLocation(blur_program, "frameSize"), 16, 16);
  glUniform2f(glGetUniformLocation(blur_program, "texSize"), 16, 16);
  glUniform1i(glGetUniformLocation(blur_program, "tex"), 0);
  v /= 50.0;
  if (v > 1.0) {
    v = 1.0;
  } else if (v < 0.0) {
    v = 0.0;
  }
  glUniform1i(glGetUniformLocation(blur_program, "tri"), false);
  glUniform1i(glGetUniformLocation(blur_program, "full"), true);
  glUniformMatrix4fv(glGetUniformLocation(blur_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(currentMatrix));
  glUniformMatrix4fv(glGetUniformLocation(blur_program, "proj"), 1, GL_FALSE,
                     glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(glGetAttribLocation(blur_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(blur_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(blur_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(blur_program, "t"));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glUseProgram(0);
}

void renderWheelTriF(int x, int y, int w, int h, float v) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, palImg);
  float r = 0.5 * sqrt(3) * 2 * w / 3.0;
  float d = r * 2 / w;
  float a = 0.4;
  x += w / 2.0;
  y += h / 2.0;
  w /= d;
  h /= d;
  w *= a * 2;
  h *= a * 2;
  glm::mat4 currentMatrix = model;
  currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, 0.0));
  currentMatrix =
      glm::rotate(currentMatrix, glm::radians(float(v)), glm::vec3(0, 0, 1));
  currentMatrix = glm::scale(currentMatrix, glm::vec3(w, h, 1.0));
  glUseProgram(blur_program);
  glUniform2f(glGetUniformLocation(blur_program, "frameSize"), 16, 16);
  glUniform2f(glGetUniformLocation(blur_program, "texSize"), 16, 16);
  glUniform1i(glGetUniformLocation(blur_program, "tex"), 0);
  glUniform1i(glGetUniformLocation(blur_program, "tri"), true);
  glUniform1i(glGetUniformLocation(blur_program, "full"), true);
  glUniform1f(glGetUniformLocation(blur_program, "v"), v);
  glUniformMatrix4fv(glGetUniformLocation(blur_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(currentMatrix));
  glUniformMatrix4fv(glGetUniformLocation(blur_program, "proj"), 1, GL_FALSE,
                     glm::value_ptr(proj));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vboTri);
  glVertexAttribPointer(glGetAttribLocation(blur_program, "position"), 2,
                        GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(blur_program, "position"));
  glVertexAttribPointer(glGetAttribLocation(blur_program, "t"), 2, GL_FLOAT,
                        GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(blur_program, "t"));
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glUseProgram(0);
}

void renderUI(int x, int y, int w, int h, GLuint tex, int cutoff, int cutoff2,
              int cutoff3, int cutoff4) {
  if (w > 0 && h > 0) {
    int tw, th;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);
    glUseProgram(my_program);
    glUniform1i(mp_tex, 0);
    glUniform1i(mp_texN, 1);
    glUniform2f(mp_texSize, (float)tw, (float)th);
    glUniform1f(mp_depth, 0.0);
    glUniform1f(mp_strength, 0.0);
    glUniform2f(mp_flip, (float)0, (float)0);
    glUniformMatrix4fv(mp_proj, 1, GL_FALSE, glm::value_ptr(proj));
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(glGetAttribLocation(my_program, "position"), 2,
                          GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(glGetAttribLocation(my_program, "position"));
    glVertexAttribPointer(glGetAttribLocation(my_program, "t"), 2, GL_FLOAT,
                          GL_FALSE, 4 * sizeof(float),
                          (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(glGetAttribLocation(my_program, "t"));

    renderGlyph(0, x, y, 2, 2, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0, false, false,
                cutoff, cutoff2, cutoff3, cutoff4);
    renderGlyph(1, x + 2, y, w - 4, 2, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0, false,
                false, cutoff, cutoff2, cutoff3, cutoff4);
    renderGlyph(2, x + w - 2, y, 2, 2, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0, false,
                false, cutoff, cutoff2, cutoff3, cutoff4);
    renderGlyph(3, x, y + 2, 2, h - 4, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0, false,
                false, cutoff, cutoff2, cutoff3, cutoff4);
    renderGlyph(4, x + 2, y + 2, w - 4, h - 4, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0,
                false, false, cutoff, cutoff2, cutoff3, cutoff4);
    renderGlyph(5, x + w - 2, y + 2, 2, h - 4, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0,
                false, false, cutoff, cutoff2, cutoff3, cutoff4);
    renderGlyph(6, x, y + h - 2, 2, 2, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0, false,
                false, cutoff, cutoff2, cutoff3, cutoff4);
    renderGlyph(7, x + 2, y + h - 2, w - 4, 2, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0,
                false, false, cutoff, cutoff2, cutoff3, cutoff4);
    renderGlyph(8, x + w - 2, y + h - 2, 2, 2, tex, 2, 2, 0, 0, 0, 1, 0, 0, 0.0,
                false, false, cutoff, cutoff2, cutoff3, cutoff4);
  }
}

int textW(const std::string& text, int x, int y, GLuint tex, bool alignRight) {
  int kerning[] = {2,  2, 2, 2, 3, 3, 2, 2, 6, 3, 3, 3, 1, 3, 2, 2, 2, 2, 3,
                   2,  3, 2, 1, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 5, 4, 3, 5,
                   -1, 2, 2, 2, 2, 2, 3, 3, 2, 2, 1, 3, 2, 2, 2, 2, 6,

                   6,  5, 3, 3, 2, 2, 6, 5, 5, 3, 3, 6, 4, 6, 2, 6, 6, 3, 4,
                   3,  2, 1, 5, 4, 5, 5, 4, 4, 6, 4, 4};
  int kerningD[] = {3, 5, 3, 3, 3, 3, 3, 4, 3, 3, 2, 4};
  int w = -1;
  for (int i = 0; i < text.size(); i++) {
    if (text[i] >= 33 && text[i] <= 47) {
      w += 9 - kerning[55 + text[i] - 33];
    } else if (text[i] >= 48 && text[i] <= 57) {
      w += 9 - kerningD[text[i] - 48];
    } else if (text[i] >= 58 && text[i] <= 64) {
      w += 9 - kerning[70 + text[i] - 58];
    } else if (text[i] >= 91 && text[i] <= 95) {
      w += 9 - kerning[77 + text[i] - 91];
    } else if (text[i] >= 123 && text[i] <= 126) {
      w += 9 - kerning[82 + text[i] - 123];
    } else if (text[i] == 32) {
      w += 9 - kerning[52];
    } else if ((text[i] >= 65 && text[i] <= 90) ||
               (text[i] >= 97 && text[i] <= 122)) {
      int f = text[i] - 'A';
      if (f >= 32) {
        f -= 6;
      }
      w += 9 - kerning[f];
    } else {
      w += 9 - kerning[0];
    }
  }
  int wd = w;
  return wd;
}

int textH(const std::string& text, int x, int y, GLuint tex, bool alignRight) {
  int h = 9;
  for (int i = 0; i < text.size(); i++) {
    if (text[i] == '\n') {
      h += 9;
    }
  }
  return h;
}

