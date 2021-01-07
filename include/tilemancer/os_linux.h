#ifndef TILEMANCER_OS_LINUX_H
#define TILEMANCER_OS_LINUX_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pwd.h>
#include <future>

extern const char* light_vertex_shader_source;
extern const char* light_fragment_shader_source;

extern const char* blur_vertex_shader_source;
extern const char* blur_fragment_shader_source;

extern const char* transition_vertex_shader_source;
extern const char* transition_fragment_shader_source;

extern const char* my_vertex_shader_source;
extern const char* my_fragment_shader_source;

extern int OS;

#endif  // TILEMANCER_OS_LINUX_H
