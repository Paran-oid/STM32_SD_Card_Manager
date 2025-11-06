#pragma once

extern "C"
{
#include "main.h"
}

#include <type_traits>
#include <vector>

#include "gpio.hpp"

/***********************************************************
 * Public defines
 ***********************************************************/
constexpr uint8_t MAX_PWM_CHANNELS = 8;

/***************************************************************
 * PTimer class
 * - precise (micro second level) 32/16-bit timer
 * --> assuming that timers use internal clock of MCU
 ***************************************************************/
template <typename T = uint32_t>
class PTimer
{
   private:
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
    static_assert(std::is_same<T, uint32_t>::value || std::is_same<T, uint16_t>::value,
                  "Timer<T>: T must be uint16_t or uint32_t");

    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    PTimer() = delete;
    PTimer(TIM_HandleTypeDef& htim) : m_htim {htim}, m_pwm_channels_state {0x0}
    {
    }

    ~PTimer()
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
    bool start();
    bool stop();

    uint32_t elapsed_us() const;
    uint32_t elapsed_ms() const;

    void delay_us(T period_us);
    void delay_ms(uint16_t period_ms);

    bool delay_until(GPIO& gpio, GPIOState expected_state, T period_us);

    // pulse-width modulation generation
    bool        start_pwm(uint8_t channel);
    inline void set_pwm(uint16_t val, uint8_t channel)
    {
        __HAL_TIM_SET_COMPARE(&m_htim, channel, val);
    }
    inline T get_pwm(uint8_t channel) const
    {
        return __HAL_TIM_GET_COMPARE(&m_htim, channel);
    }

    bool stop_pwm(uint8_t channel);

    inline void reset() const
    {
        __HAL_TIM_SET_COUNTER(&m_htim, 0);
    }
};
