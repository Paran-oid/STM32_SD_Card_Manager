#include "hal_init.hpp"

#include "iwdg.hpp"
#include "ptimer.hpp"
#include "spi.hpp"
#include "uart.hpp"

sca::iwdg             iwdg  = {hiwdg};
sca::spi              spi1  = {hspi1};
sca::ptimer<uint32_t> tim2  = {htim2};
sca::uart             uart2 = {huart2};

MicroSDReader sd_reader = {hspi1};

void hal_init_all()
{
    // initialize any peripheral (start timer i.e)
}
