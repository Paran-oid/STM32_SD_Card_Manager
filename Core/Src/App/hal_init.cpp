#include "hal_init.hpp"

#include "_iwdg.hpp"
#include "ptimer.hpp"

PTimer<uint32_t> tim2      = {htim2};
_IWDG            iwdg      = {hiwdg};
MicroSDReader    sd_reader = {hspi1};

void hal_init_all()
{
    // initialize any peripheral (start timer i.e)
}
