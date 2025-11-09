#include "gpio.hpp"

namespace sca
{

void gpio::mode_set(GPIOMode mode)
{
    GPIO_InitTypeDef gpio_initializer;

    gpio_initializer.Pin  = m_pin;
    gpio_initializer.Mode = static_cast<uint32_t>(mode);
    gpio_initializer.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(m_port, &gpio_initializer);
}

void gpio::set_state(GPIOState state)
{
    HAL_GPIO_WritePin(m_port, m_pin, (GPIO_PinState) state);
}

void gpio::toggle_state()
{
    HAL_GPIO_TogglePin(m_port, m_pin);
}

gpio::GPIOState gpio::get_state() const
{
    return HAL_GPIO_ReadPin(m_port, m_pin) == GPIO_PIN_RESET ? GPIOState::LOW : GPIOState::HIGH;
}

}  // namespace sca
