#pragma once

extern "C"
{
#include "stm32l4xx.h"
}

#include "defs.hpp"

namespace sca
{

#ifdef SCA_IWDG

/***************************************************************
 * iwdg class for managing independent watchdog timer
 ****************************************************************/
class iwdg
{
   private:
    /***********************************************************
     * Private Members
     ***********************************************************/
    IWDG_HandleTypeDef& m_hiwdg;

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    iwdg() = delete;
    iwdg(IWDG_HandleTypeDef& hiwdg) : m_hiwdg {hiwdg}
    {
    }

    ~iwdg() = default;

    /***********************************************************
     * Public Methods
     ***********************************************************/
    SCA_RES refresh();
};

#endif

}  // namespace sca
