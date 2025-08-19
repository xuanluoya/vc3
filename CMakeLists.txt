cmake_minimum_required(VERSION 3.15)

project(vc3
  VERSION 0.1.0
  DESCRIPTION "LC-3 Virtual Machine"
  LANGUAGES C
)

set(TARGET_NAME ${PROJECT_NAME})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type." FORCE)
        message(STATUS "No build type selected, default to Debug")
    endif()
endif()

# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
  src/main.c
  src/core/util.c
  src/core/log.c
  src/vm/vm.c
  src/vm/instr.c
  src/vm/io.c
  src/vm/loader.c
  src/vm/memory.c
)

add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(${TARGET_NAME} PRIVATE
  $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra -pedantic>
  $<$<C_COMPILER_ID:MSVC>:/W4 /permissive->
  $<$<CONFIG:Debug>:-O0 -g -DDEBUG>
  $<$<CONFIG:Release>:-O3 -DNDEBUG>
  $<$<CONFIG:RelWithDebInfo>:-O2 -g -DNDEBUG>
  $<$<CONFIG:MinSizeRel>:-Os -DNDEBUG>
)

set_target_properties(${TARGET_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION bin)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include FILES_MATCHING PATTERN "*.h")

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Build finished: $<TARGET_FILE:${TARGET_NAME}>"
  COMMAND ${CMAKE_COMMAND} -E echo "Build type: ${CMAKE_BUILD_TYPE}"
  COMMAND ${CMAKE_COMMAND} -E echo "Version: ${PROJECT_VERSION}"
)
