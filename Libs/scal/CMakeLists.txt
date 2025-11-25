add_library(scal STATIC)

target_sources(
  scal
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/ptimer.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/src/iwdg.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/src/spi.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/src/uart.cpp)

target_include_directories(scal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(scal PRIVATE stm32cubemx etl::etl)
