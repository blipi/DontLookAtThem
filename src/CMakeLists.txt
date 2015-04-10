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

macro(copy_dirs GLOBPAT DESTINATION)
  file(GLOB COPY_DIRS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLOBPAT})
  add_custom_target(resources ALL
    DEPENDS ${GLOBPAT}
	
    COMMENT "Copying files: ${GLOBPAT}")

  foreach(FILENAME ${COPY_DIRS})
    set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
    set(DST "${DESTINATION}")

    add_custom_command(
      TARGET resources
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${DST}
    )

    add_custom_command(
      TARGET resources
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${SRC} ${DST}
    )

    add_dependencies(resources DontLookAtThem)
  endforeach(FILENAME)
endmacro(copy_dirs)

copy_dirs(${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:DontLookAtThem>/resources)
