#include "sca/ptimer.hpp"

#include <etl/limits.h>

#include "sca/gpio.hpp"

namespace sca
{

#ifdef SCA_PTIMER

template class ptimer<uint32_t>;
template class ptimer<uint16_t>;

const TIM_TypeDef* ABP2_TIMS[] = {TIM1, TIM8};

template <typename T>
uint32_t ptimer<T>::get_freq() const
{
    uint32_t pclk;  // peripheral clock

    for (const auto& tim : ABP2_TIMS)
    {
        if (m_htim.Instance == tim)
        {
            pclk = HAL_RCC_GetPCLK2Freq();
            if ((RCC->CFGR & RCC_CFGR_PPRE2) != RCC_CFGR_PPRE2_DIV1) pclk *= 2;
            return pclk / (m_htim.Instance->PSC + 1);
        }
    }

    pclk = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) pclk *= 2;

    return pclk / (m_htim.Instance->PSC + 1);
}

template <typename T>
SCA_RES ptimer<T>::start()
{
    return HAL_TIM_Base_Start(&m_htim) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

template <typename T>
SCA_RES ptimer<T>::stop()
{
    return HAL_TIM_Base_Stop(&m_htim) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

template <typename T>
uint32_t ptimer<T>::elapsed_us() const
{
    uint32_t ticks = __HAL_TIM_GET_COUNTER(&m_htim);
    uint32_t freq  = this->get_freq();

    return ticks / (freq / 1'000'000);
}

template <typename T>
uint32_t ptimer<T>::elapsed_ms() const
{
    uint32_t ticks = __HAL_TIM_GET_COUNTER(&m_htim);
    uint32_t freq  = this->get_freq();

    return ticks / (freq / 1'000);
}

template <typename T>
void ptimer<T>::delay_us(T timeout_us)
{
    uint32_t m_freq           = this->get_freq();
    uint64_t calculated_ticks = timeout_us * (m_freq / 1'000'000);
    if (calculated_ticks > etl::numeric_limits<T>::max()) return;  // error encountered

    uint32_t start = __HAL_TIM_GetCounter(&m_htim);
    while ((__HAL_TIM_GetCounter(&m_htim) - start) < calculated_ticks);
}

template <typename T>
void ptimer<T>::delay_ms(uint16_t timeout_ms)
{
    uint32_t m_freq           = this->get_freq();
    uint64_t calculated_ticks = timeout_ms * (m_freq / 1'000);
    if (calculated_ticks > etl::numeric_limits<T>::max()) return;  // error encountered

    uint32_t start = __HAL_TIM_GetCounter(&m_htim);
    while ((__HAL_TIM_GetCounter(&m_htim) - start) < calculated_ticks);
}

template <typename T>
SCA_RES ptimer<T>::delay_until(gpio& gpio, GPIOState expected_state, T timeout_us)
{
    this->reset();

    while (gpio.get_state() != expected_state)
    {
        if (this->elapsed_us() >= timeout_us) return SCA_RES::ERR;
    }

    return SCA_RES::OK;
}

template <typename T>
SCA_RES ptimer<T>::start_pwm(uint8_t channel)
{
    if (channel >= MAX_PWM_CHANNELS) return SCA_RES::ERR;

    if (m_pwm_channels_state & (1 << channel)) return SCA_RES::OK;  // already running
    m_pwm_channels_state |= (1 << channel);
    return HAL_TIM_PWM_Start(&m_htim, channel) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

template <typename T>
SCA_RES ptimer<T>::stop_pwm(uint8_t channel)
{
    if (channel >= MAX_PWM_CHANNELS) return SCA_RES::ERR;

    m_pwm_channels_state &= ~(1 << channel);
    return HAL_TIM_PWM_Stop(&m_htim, channel) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

#endif

}  // namespace sca
