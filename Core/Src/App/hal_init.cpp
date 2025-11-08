#include "hal_init.hpp"

#include "_iwdg.hpp"
#include "ptimer.hpp"
#include "spi.hpp"
#include "uart.hpp"

_IWDG            iwdg  = {hiwdg};
SPI              spi1  = {hspi1};
PTimer<uint32_t> tim2  = {htim2};
UART             uart2 = {huart2};

MicroSDReader sd_reader = {hspi1};

void hal_init_all()
{
    // initialize any peripheral (start timer i.e)
}
