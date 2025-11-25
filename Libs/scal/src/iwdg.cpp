#include "scal/iwdg.hpp"

namespace scal
{

#ifdef SCA_IWDG

SCA_RES iwdg::refresh()
{
    return HAL_IWDG_Refresh(&m_hiwdg) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

#endif

}  // namespace scal
