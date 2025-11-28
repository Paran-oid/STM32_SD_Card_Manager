#include "hal_init.hpp"

#include "filesystem.hpp"
#include "scal/iwdg.hpp"
#include "scal/ptimer.hpp"
#include "scal/spi.hpp"
#include "scal/uart.hpp"

namespace fs = stm_sd::filesystem;

scal::SPI              spi1  = {hspi1};
scal::PTimer<uint32_t> tim2  = {htim2};
scal::UART             uart2 = {huart2};

namespace stm_sd
{

void HALInit()
{
    // initialize any peripheral (start timer i.e)

    fs::init(hspi1);  // set hspi1 as spi periph for filesystem
}

}  // namespace stm_sd
