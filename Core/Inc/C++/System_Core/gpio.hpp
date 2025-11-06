#pragma once

extern "C"
{
#include "main.h"
}

/***************************************************************
 * GPIO class for handling general-purpose register interactions
 ***************************************************************/
class GPIO
{
   private:
    /***********************************************************
     * Private Members
     ***********************************************************/
    GPIO_TypeDef* m_port;
    uint16_t      m_pin;

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    GPIO() = delete;
    GPIO(GPIO_TypeDef* port, uint16_t pin) : m_port {port}, m_pin {pin}
    {
    }

    ~GPIO() = default;

    /***************************************************************
     * Public GPIO enums for state and mode management
     ***************************************************************/
    enum class GPIOMode : uint8_t
    {
        OUTPUT = GPIO_MODE_OUTPUT_PP,
        INPUT  = GPIO_MODE_INPUT
    };

    enum class GPIOState : uint8_t
    {
        HIGH = GPIO_PIN_SET,
        LOW  = GPIO_PIN_RESET
    };

    /***********************************************************
     * Getters and Setters
     ***********************************************************/
    void mode_set(GPIOMode mode);

    void      set_state(GPIOState state);
    void      toggle_state();
    GPIOState get_state() const;

    inline uint16_t get_pin() const
    {
        return m_pin;
    }

    inline GPIO_TypeDef* get_port() const
    {
        return m_port;
    }
};
