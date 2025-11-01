extern "C"
{
#include "main.h"
}

void setup()
{
}

void loop()
{
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(500);
}

extern "C"
{
    void setup_map(void)
    {
        setup();
    }

    void loop_map(void)
    {
        loop();
    }
}
