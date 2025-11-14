cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once, and is not re-generated if converter is
# called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32_SD_Card_Manager)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Packages and libraries
add_subdirectory(Libs)

# Software testing directory (just software)
add_subdirectory(SW)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(
  ${CMAKE_PROJECT_NAME} PRIVATE # Add user defined library search paths
)

# Add sources to executable
target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE #
          # Add user sources here
          #
          ${CMAKE_SOURCE_DIR}/Core/Src/entry.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/utils.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/Modules/sd_file.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/Modules/sd_reader.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/hal_init.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/command_handler.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/Commands/echo.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/Commands/rm.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/Commands/common_cmds.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/Commands/cat.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/Commands/cp.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/Commands/ls.cpp
          ${CMAKE_SOURCE_DIR}/Core/Src/App/Commands/cd.cpp
          #
          # Test-related
          #
          ${CMAKE_SOURCE_DIR}/Core/Tests/test_sd.cpp
          ${CMAKE_SOURCE_DIR}/Core/Tests/tests.cpp
          #
          # Added micro sd spi "implementation"
          #
          ${CMAKE_SOURCE_DIR}/FATFS/Target/fatfs_sd.c)

# Add include paths
target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PRIVATE #
          # Add user defined include paths
          #
          ${CMAKE_SOURCE_DIR}/Core/Inc
          ${CMAKE_SOURCE_DIR}/Core/Inc/Modules
          ${CMAKE_SOURCE_DIR}/Core/Inc/App
          #
          # Test-related
          #
          ${CMAKE_SOURCE_DIR}/Core/Tests)

# Add project symbols (macros)
target_compile_definitions(
  ${CMAKE_PROJECT_NAME} PUBLIC # Add user defined symbols
)

target_compile_definitions(
  sca PUBLIC # Add user defined symbols for cpp abstraction library
             SCA_GPIO SCA_SPI SCA_PTIMER SCA_UART)

# Add flags
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -fno-rtti
                                                     -fno-exceptions)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(
  ${CMAKE_PROJECT_NAME} stm32cubemx etl::etl # Add user defined libraries
  sca printf)
