#include "iwdg.hpp"

namespace sca
{

SCA_RES iwdg::refresh()
{
    return HAL_IWDG_Refresh(&m_hiwdg) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

}  // namespace sca
