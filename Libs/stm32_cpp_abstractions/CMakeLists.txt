add_library(stm32_cpp_abstractions STATIC)

target_sources(
  stm32_cpp_abstractions
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Timers/ptimer.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/System_Core/iwdg.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/System_Core/gpio.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Connectivity/spi.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Connectivity/uart.cpp)

target_include_directories(
  stm32_cpp_abstractions
  INTERFACE
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/System_Core
         ${CMAKE_CURRENT_SOURCE_DIR}/Connectivity
         ${CMAKE_CURRENT_SOURCE_DIR}/Timers)

target_link_libraries(stm32_cpp_abstractions PRIVATE stm32cubemx etl::etl)
