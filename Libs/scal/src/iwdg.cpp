#include "sca/iwdg.hpp"

namespace sca
{

#ifdef SCA_IWDG

SCA_RES iwdg::refresh()
{
    return HAL_IWDG_Refresh(&m_hiwdg) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

#endif

}  // namespace sca
