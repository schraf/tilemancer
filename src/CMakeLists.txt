
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
    load_texture.cc
    main.cc
    math.cc
    palette.cc
    parameter.cc
    render.cc
    saveload.cc
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

target_include_directories(tilemancer PRIVATE
    ../include
    ../external/glew/include
    ../external/sdl2/include)

target_link_libraries(tilemancer
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${CMAKE_DL_LIBS}
                      ${OPENGL_LIBRARIES}
                      sdl2
                      glm
                      glew
                      lua
                      stb
                      )
