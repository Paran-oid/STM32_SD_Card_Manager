#pragma once

#include "defines.hpp"

class MicroSDReader
{
   private:
   public:
    MicroSDReader() = delete;

    MP_RES detect();
    MP_RES mount();
};
