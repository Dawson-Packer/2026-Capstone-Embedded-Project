#include "power.hpp"
#include "Utils/logging.hpp"
#include "rtc.h"

static const char TAG[] = "power";

bool Power::WokeFromStandby(void)
{
    bool woke = __HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET;
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    return woke;
}

void Power::EnterStandby(uint32_t next_wakeup_seconds)
{
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

    /* Clear flags */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, next_wakeup_seconds, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

    LOG_INF(TAG, "Entering standby mode. Configured to wake up in %lu seconds...",
            next_wakeup_seconds);

    HAL_PWR_EnterSTANDBYMode();
}