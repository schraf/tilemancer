cmake_minimum_required(VERSION 3.1.0)
project(tilemancer C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# compiler flags
if (MSVC)
	add_definitions(/D _CRT_SECURE_NO_WARNINGS)

	# character set to Not Set=ASCII/SBCS
  # http://public.kitware.com/Bug/view.php?id=12189
	add_definitions(/D _SBCS)
endif()

add_subdirectory(external)
add_subdirectory(src)
