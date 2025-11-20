#include "hal_init.hpp"

#include "filesystem.hpp"
#include "sca/iwdg.hpp"
#include "sca/ptimer.hpp"
#include "sca/spi.hpp"
#include "sca/uart.hpp"

namespace fs = stm_sd::filesystem;

sca::spi              spi1  = {hspi1};
sca::ptimer<uint32_t> tim2  = {htim2};
sca::uart             uart2 = {huart2};

namespace stm_sd
{

void hal_init_all()
{
    // initialize any peripheral (start timer i.e)
    fs::init(hspi1);
}

}  // namespace stm_sd
