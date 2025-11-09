#pragma once

extern "C"
{
#include "stm32l4xx.h"
}

#include <etl/type_traits.h>

#include "defs.hpp"
#include "gpio.hpp"

namespace sca
{

/***************************************************************
 * ptimer class
 * - precise (micro second level) 32/16-bit timer
 * --> assuming that timers use internal clock of MCU
 ***************************************************************/
template <typename T = uint32_t>
class ptimer
{
   private:
    /***********************************************************
     * Private Defines
     ***********************************************************/
    static constexpr uint8_t MAX_PWM_CHANNELS = 8;

    /***********************************************************
     * Private Members
     ***********************************************************/
    TIM_HandleTypeDef& m_htim;
    uint8_t            m_pwm_channels_state;
    /***********************************************************
     * Private Methods
     ***********************************************************/
    uint32_t get_freq() const;  // returns frequency of timer

   public:
    static_assert(etl::is_same_v<T, uint32_t> || etl::is_same_v<T, uint16_t>,
                  "Timer<T>: T must be uint16_t or uint32_t");

    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    ptimer() = delete;
    ptimer(TIM_HandleTypeDef& htim) : m_htim {htim}, m_pwm_channels_state {0x0}
    {
    }

    ~ptimer()
    {
        // stop any channel that is still running pwm
        for (uint8_t channel = 0; channel < 8; channel++)
        {
            if (m_pwm_channels_state & (1 << channel)) this->stop_pwm(channel);
        }
        this->stop();
    };

    /***********************************************************
     * Public Methods
     ***********************************************************/
    SCA_RES start();
    SCA_RES stop();

    uint32_t elapsed_us() const;
    uint32_t elapsed_ms() const;

    void delay_us(T period_us);
    void delay_ms(uint16_t period_ms);

    SCA_RES delay_until(gpio& gpio, gpio::GPIOState expected_state, T period_us);

    // pulse-width modulation generation
    SCA_RES     start_pwm(uint8_t channel);
    inline void set_pwm(uint16_t val, uint8_t channel)
    {
        (void) __HAL_TIM_SET_COMPARE(&m_htim, channel, val);
    }
    inline T get_pwm(uint8_t channel) const
    {
        return __HAL_TIM_GET_COMPARE(&m_htim, channel);
    }

    SCA_RES stop_pwm(uint8_t channel);

    inline void reset() const
    {
        __HAL_TIM_SET_COUNTER(&m_htim, 0);
    }
};

}  // namespace sca
