#include "utils.hpp"

#include "printf.h"

void die(etl::string_view msg)
{
    printf("%s\r\n", msg.data());
    Error_Handler();
}
