#ifndef TILEMANCER_OS_OSX_H
#define TILEMANCER_OS_OSX_H

#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <mach-o/dyld.h>
#include <pwd.h>
#include <future>
#include "CoreFoundation/CFBundle.h"
const char* light_vertex_shader_source = (char*)"#version 330\nin vec4 position; uniform int frame; uniform vec2 frameSize; uniform vec2 off; uniform vec2 texSize; uniform mat4 model; uniform mat4 proj; in vec2 t; out vec2 txc; void main() { gl_Position = (proj*model) * position; int tx = frame%5; int ty = frame/5; txc = vec2(((t.x+tx)*frameSize.x+off.x)/texSize.x, ((t.y+ty)*frameSize.y+off.y)/texSize.y); }";
const char* light_fragment_shader_source = (char*)"#version 330\nlayout(location = 0) out vec4 fragColor; uniform vec3 color; uniform vec3 color2; in vec2 txc; void main() { fragColor = vec4(color*(1.0-txc.x)+color2*txc.x, 1.0); }";

const char* blur_vertex_shader_source = (char*)"#version 330\nin vec4 position; uniform int frame; uniform vec2 frameSize; uniform vec2 texSize; uniform mat4 model; uniform mat4 proj; in vec2 t; out vec2 txc; void main() { gl_Position = (proj*model) * position; int tx = frame%5; int ty = frame/5; txc = vec2((t.x+tx)*frameSize.x/texSize.x, (t.y+ty)*frameSize.y/texSize.y); }";
const char* blur_fragment_shader_source = (char*)"#version 330\n#define M_PI 3.1415926535897932384626433832795\nlayout(location = 0) out vec4 fragColor; in vec2 txc; uniform sampler2D tex; uniform vec2 texSize; uniform float v; uniform bool tri; uniform bool full; void main() { float a = 0.0; vec2 dist = vec2(0.5, 0.5)-txc; a = ((length(dist)>0.5) ? 0.0 : 1.0); a = ((length(dist)<0.4) ? 0.0 : a); a = (tri ? 1.0 : a); float H = atan(dist.y, dist.x)/M_PI*180.0+180.0; H = (H==360 ? 0.0 : H); H = (tri ? v : H); float S = (tri ? txc.x/txc.y : 1.0); float V = (tri ? txc.y : 1.0); float C = V*S; float X = C*(1.0-abs(mod(H/60.0, 2)-1.0)); float m = V-C; vec3 rgb = vec3(0.0, 0.0, 0.0); if(0 <= H && H < 60) { rgb = vec3(C, X, 0.0); } else if(60 <= H && H < 120) { rgb = vec3(X, C, 0.0); } else if(120 <= H && H < 180) { rgb = vec3(0.0, C, X); } else if(180 <= H && H < 240) { rgb = vec3(0.0, X, C); } else if(240 <= H && H < 300) { rgb = vec3(X, 0.0, C); } else if(300 <= H && H < 360) { rgb = vec3(C, 0.0, X); } rgb = rgb+m; float d = 10.0; vec3 finalColor = vec3(0.0, 0.0, 0.0); for(int x = 0; x < texSize.x; x++) { for(int y = 0; y < texSize.y; y++) { vec3 color = vec3(texture(tex, vec2(x/texSize.x, y/texSize.y))); vec3 dist = rgb-color; float d2 = length(dist); if(d2 < d) { d = d2; finalColor = color; } } } if(full) { finalColor = rgb; }  fragColor = vec4(finalColor, a); }";

const char* transition_vertex_shader_source = (char*)"#version 330\nin vec4 position; uniform int frame; uniform vec2 frameSize; uniform vec2 texSize; uniform mat4 model; uniform mat4 proj; in vec2 t; out vec2 txc; void main() { gl_Position = (proj*model) * position; int tx = frame%5; int ty = frame/5; txc = vec2((t.x+tx)*frameSize.x/texSize.x, (t.y+ty)*frameSize.y/texSize.y); }";
const char* transition_fragment_shader_source = (char*)"#version 330\nlayout(location = 0) out vec4 fragColor; uniform float rot; uniform vec2 texSize; uniform sampler2D tex; uniform sampler2D texP; in vec2 txc; void main() { vec3 off = vec3(-1.0/texSize.x, 0.0, 1.0/texSize.x); vec2 size = vec2(2.0, 0.0); float s11 = texture(tex, txc).x; float s01 = texture(tex, txc+off.xy).x; float s21 = texture(tex, txc+off.zy).x; float s10 = texture(tex, txc+off.yx).x; float s12 = texture(tex, txc+off.yz).x; vec3 va = normalize(vec3(size.xy,s21-s01)); vec3 vb = normalize(vec3(size.yx,s12-s10)); vec3 bump = vec3(cross(va,vb)); vec3 light = vec3(sin(rot), -cos(rot), 0.3); float d = max(0.0, dot(normalize(light), bump)); vec2 ptxc = vec2(d, 0.0); vec3 colorFinal = vec3(texture(texP, ptxc)); fragColor = vec4(colorFinal, 1.0)";

const char* my_vertex_shader_source = (char*)"#version 330\nin vec4 position; uniform int frame; uniform vec2 frameSize; uniform vec2 off; uniform vec2 texSize; uniform mat4 model; uniform mat4 proj; in vec2 t; out vec2 txc; void main() { gl_Position = (proj*model) * position; int tx = frame%5; int ty = frame/5; txc = vec2(((t.x+tx)*frameSize.x+off.x)/texSize.x, ((t.y+ty)*frameSize.y+off.y)/texSize.y); }";
const char* my_fragment_shader_source = (char*)"#version 330\nlayout(location = 0) out vec4 fragColor; layout(location = 1) out vec4 fragColorN; layout(location = 2) out vec4 fragColorMisc; layout(location = 3) out vec4 fragColorB; uniform vec2 flip; uniform sampler2D tex; uniform float strength; uniform float alpha; uniform float depth; uniform sampler2D texN; in vec2 txc; void main() { fragColor = vec4(texture(tex, txc).xyz/texture(tex, txc).w, texture(tex, txc).w*alpha); }";
CFBundleRef mainBundle;

int OS = Apple;

#endif  // TILEMANCER_OS_OSX_H
