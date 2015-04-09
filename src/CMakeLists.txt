# Based on TrinityCore CMake files
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources_Game Game/*.cpp     Game/*.hpp)

set(EPS_SRCS
  ${EPS_SRCS}
  ${sources_Game}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Game

  ${ModernGameEngine_INCLUDE}
)

add_definitions(${ModernGameEngine_DEFINITIONS})

add_executable(DontLookAtThem
  ${EPS_SRCS}
)

target_link_libraries(DontLookAtThem
  ${ModernGameEngine_LIBRARIES}
)
