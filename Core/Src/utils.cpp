#include "utils.hpp"

#include "printf.h"

void die(etl::string_view msg)
{
    printf("%s", msg.data());
    Error_Handler();
}
