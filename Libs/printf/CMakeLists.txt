add_library(printf)

target_sources(printf PRIVATE printf.c)

target_include_directories(printf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
