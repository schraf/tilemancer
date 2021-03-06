
add_executable(tilemancer
    bezier.cc
    bpoint.cc
    browser.cc
    browserfile.cc
    color.cc
    cpoint.cc
    drag.cc
    effect.cc
    floatimage.cc
    globals.cc
    graphics_globals.cc
    image.cc
    load_texture.cc
    main.cc
    math.cc
    palette.cc
    parameter.cc
    render.cc
    saveload.cc
    shaders.cc
    socket.cc
    stringutils.cc
    texture.cc
    tilemancer.cc
    undoredo.cc)

set_target_properties(tilemancer
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dist"
                      DEBUG_OUTPUT_NAME "tilemancer-debug"
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/dist"
                      RELEASE_OUTPUT_NAME "tilemancer"
                      )

target_include_directories(tilemancer PUBLIC
    ../include
    ../external/glew/include
    ../external/sdl2/include
    ../external/glm/include
    ../external/lua/src
    ../external/stb
    ../external/tinydir)

target_link_libraries(tilemancer
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${CMAKE_DL_LIBS}
                      ${OPENGL_LIBRARIES}
                      SDL2main SDL2-static
                      glew_s
                      lua
                      )
