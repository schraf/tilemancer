set(SDL_SHARED_ENABLED_BY_DEFAULT OFF)
set(BUILD_UTILS OFF)

add_subdirectory(glew/build/cmake)
add_subdirectory(glm)
add_subdirectory(lua)
add_subdirectory(sdl2)
add_subdirectory(stb)
