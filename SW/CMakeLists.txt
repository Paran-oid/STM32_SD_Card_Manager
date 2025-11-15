cmake_minimum_required(VERSION 3.28.3)
project(SW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_subdirectory(Libs)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/main.cpp
          ${CMAKE_SOURCE_DIR}/Src/entry.cpp
          ${CMAKE_SOURCE_DIR}/Src/utils.cpp
          ${CMAKE_SOURCE_DIR}/Src/App/command_handler.cpp
          ${CMAKE_SOURCE_DIR}/Src/App/Commands/echo.cpp
          ${CMAKE_SOURCE_DIR}/Src/App/Commands/rm.cpp
          ${CMAKE_SOURCE_DIR}/Src/App/Commands/common_cmds.cpp
          ${CMAKE_SOURCE_DIR}/Src/App/Commands/cat.cpp
          ${CMAKE_SOURCE_DIR}/Src/App/Commands/cp.cpp
          ${CMAKE_SOURCE_DIR}/Src/App/Commands/ls.cpp
          ${CMAKE_SOURCE_DIR}/Src/App/Commands/cd.cpp)

target_include_directories(
  ${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Inc
                                ${CMAKE_SOURCE_DIR}/Inc/App)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE etl::etl)

target_compile_options(
  ${CMAKE_PROJECT_NAME}
  PRIVATE -Wall
          -Wall
          -Wextra
          -Werror
          -Wshadow
          -Wnon-virtual-dtor
          -Wold-style-cast
          -Wunused
          -Wconversion
          -Wsign-conversion)
