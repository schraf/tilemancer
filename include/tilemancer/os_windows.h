#ifndef TILEMANCER_OS_WINDOWS_H
#define TILEMANCER_OS_WINDOWS_H

#define GLEW_STATIC
#include <GL\glew.h>
#include <SDL.h>
#include "stb_image.h"

const char* light_vertex_shader_source = (char*)"#version 130\nin vec4 position; uniform int frame; uniform vec2 frameSize; uniform vec2 off; uniform vec2 texSize; uniform mat4 model; uniform mat4 proj; in vec2 t; out vec2 txc; void main() { gl_Position = (proj*model) * position; int tx = int(mod(frame, 5)); int ty = frame/5; txc = vec2(((t.x+tx)*frameSize.x+off.x)/texSize.x, ((t.y+ty)*frameSize.y+off.y)/texSize.y); }";
const char* light_fragment_shader_source = (char*)"#version 130\nout vec4 fragColor; uniform vec3 color; uniform vec3 color2; in vec2 txc; void main() { fragColor = vec4(color*(1.0-txc.x)+color2*txc.x, 1.0); }";

const char* blur_vertex_shader_source = (char*)"#version 130\nin vec4 position; uniform int frame; uniform vec2 frameSize; uniform vec2 texSize; uniform mat4 model; uniform mat4 proj; in vec2 t; out vec2 txc; void main() { gl_Position = (proj*model) * position; int tx = int(mod(frame, 5)); int ty = frame/5; txc = vec2((t.x+tx)*frameSize.x/texSize.x, (t.y+ty)*frameSize.y/texSize.y); }";
const char* blur_fragment_shader_source = (char*)"#version 130\n#define M_PI 3.1415926535897932384626433832795\nout vec4 fragColor; in vec2 txc; uniform sampler2D tex; uniform vec2 texSize; uniform float v; uniform bool tri; uniform bool full; void main() { float a = 0.0; vec2 dist = vec2(0.5, 0.5)-txc; a = ((length(dist)>0.5) ? 0.0 : 1.0); a = ((length(dist)<0.4) ? 0.0 : a); a = (tri ? 1.0 : a); float H = atan(dist.y, dist.x)/M_PI*180.0+180.0; H = (H==360 ? 0.0 : H); H = (tri ? v : H); float S = (tri ? txc.x/txc.y : 1.0); float V = (tri ? txc.y : 1.0); float C = V*S; float X = C*(1.0-abs(mod(H/60.0, 2)-1.0)); float m = V-C; vec3 rgb = vec3(0.0, 0.0, 0.0); if(0 <= H && H < 60) { rgb = vec3(C, X, 0.0); } else if(60 <= H && H < 120) { rgb = vec3(X, C, 0.0); } else if(120 <= H && H < 180) { rgb = vec3(0.0, C, X); } else if(180 <= H && H < 240) { rgb = vec3(0.0, X, C); } else if(240 <= H && H < 300) { rgb = vec3(X, 0.0, C); } else if(300 <= H && H < 360) { rgb = vec3(C, 0.0, X); } rgb = rgb+m; float d = 10.0; vec3 finalColor = vec3(0.0, 0.0, 0.0); for(int x = 0; x < texSize.x; x++) { for(int y = 0; y < texSize.y; y++) { vec3 color = vec3(texture2D(tex, vec2(x/texSize.x, y/texSize.y))); vec3 dist = rgb-color; float d2 = length(dist); if(d2 < d) { d = d2; finalColor = color; } } } if(full) { finalColor = rgb; }  fragColor = vec4(finalColor, a); }";

const char* transition_vertex_shader_source = (char*)"#version 130\nin vec4 position; uniform int frame; uniform vec2 frameSize; uniform vec2 texSize; uniform mat4 model; uniform mat4 proj; uniform float tSize; uniform float aspect; uniform vec2 count; uniform int on; uniform vec2 resolution; uniform vec2 ij; uniform float time; in vec2 t; out vec2 txc; void main() { vec2 totalSize = vec2(tSize, tSize)*count; vec2 p = vec2(ij.x*(tSize-1.0)+tSize/2.0, ij.y*(tSize-1.0)+tSize/2.0)+(resolution-totalSize)/2.0; float dx = 5.0; float dy = 5.0/aspect; float sizeX = 1.0-((count.x-ij.x)*(1.0-on)+ij.x*on+dx-time)/dx; float sizeY = 1.0-((count.y-ij.y)*(1.0-on)+ij.y*on+dy-time/aspect)/dy; float size = clamp(sizeX/2.0+sizeY/2.0, 0.0, 1.0); gl_Position = (proj*model) * vec4((position.xy-0.5)*vec2(tSize, tSize)*size+p, position.zw); txc = t; }";
const char* transition_fragment_shader_source = (char*)"#version 130\nlayout(location = 0) out vec4 fragColor; uniform sampler2D tex; in vec2 txc; void main() { fragColor = texture2D(tex, txc); }";

const char* my_vertex_shader_source = (char*)"#version 130\nin vec4 position; uniform int frame; uniform vec2 frameSize; uniform vec2 off; uniform vec2 texSize; uniform mat4 model; uniform mat4 proj; in vec2 t; out vec2 txc; void main() { gl_Position = (proj*model) * position; int tx = int(mod(frame, 5)); int ty = frame/5; txc = vec2(((t.x+tx)*frameSize.x+off.x)/texSize.x, ((t.y+ty)*frameSize.y+off.y)/texSize.y); }";
const char* my_fragment_shader_source = (char*)"#version 130\n uniform sampler2D tex; in vec2 txc; out vec4 fragColor; uniform float alpha; void main() { fragColor = vec4(texture2D(tex, txc).xyz, texture2D(tex, txc).w*alpha); }";

int OS = Windows;

#endif  // TILEMANCER_OS_WINDOWS_H
