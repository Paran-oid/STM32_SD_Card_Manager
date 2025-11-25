#include "hal_init.hpp"

#include "filesystem.hpp"
#include "scal/iwdg.hpp"
#include "scal/ptimer.hpp"
#include "scal/spi.hpp"
#include "scal/uart.hpp"

namespace fs = stm_sd::filesystem;

scal::spi              spi1  = {hspi1};
scal::ptimer<uint32_t> tim2  = {htim2};
scal::uart             uart2 = {huart2};

namespace stm_sd
{

void hal_init_all()
{
    // initialize any peripheral (start timer i.e)
    fs::init(hspi1);
}

}  // namespace stm_sd
