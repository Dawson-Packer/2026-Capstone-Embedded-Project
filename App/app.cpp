#include "app.hpp"
#include "Modules/Filesystem/filesystem.hpp"
#include "Modules/Measure/measure.hpp"
#include "Modules/Power/power.hpp"
#include "Modules/Transmit/transmit.hpp"
#include "Utils/logging.hpp"
#include <stdio.h>

#define TRANSMIT_INTERVAL_MINS 26 /* 4 minutes timeout, so 30 min total */

static void Task_Measure(void)
{
    record_t record = Measure::Take();
    Filesystem::Write(&record);
    Transmit::EnQueue(&record);
}

extern "C" void App_Init(void)
{
    printf(ANSI_RED "App initializing..." ANSI_RESET "\r\n");

    Transmit::Init();
    Filesystem::Init();

    // if (Power::ResetAfterBrownOut())
    // {
    //     printf(ANSI_BRIGHT_GRAY "Brown-out occurred. Board successfully reset.");
    //     power_event_t event = ::BROWN_OUT;

    //     Transmit::Notify(&event);
    // }
}
extern "C" void App_Run(void)
{
    if (Power::WokeFromStandby())
    {
        printf(ANSI_BRIGHT_BLUE ANSI_BOLD "Woke up from stand by. Taking measurement..." ANSI_RESET
                                          "\r\n");
        Task_Measure();

        Filesystem::Deinit();
        Power::EnterStandby(TRANSMIT_INTERVAL_MINS * 60); /* 45 minute intervals */
    }
    else
    {   /* Cold boot */
        // clang-format off
        printf(ANSI_BRIGHT_BLUE ANSI_BOLD "\r\n");
        printf(R"(  ___ __ ___ ___    __ ___   __   __ __  __  __  _ _ _____ __  ___ )" "\r\n");
        printf(R"( (_  /  (_  / __|  / _] _,\/' _/ |  V  |/__\|  \| | |_   _/__\| _ \ )" "\r\n");
        printf(R"(  / / // / / ,_ \ | [/\ v_/`._`. | \_/ | \/ | | ' | | | || \/ | v / )" "\r\n");
        printf(R"( |___\__/___\___/  \__/_|  |___/ |_| |_|\__/|_|\__|_| |_| \__/|_|_\ )" "\r\n");
        printf(ANSI_RESET "\r\n");
        // clang-format ons
        Transmit::Setup();
        Task_Measure();

        Filesystem::Deinit();
        Power::EnterStandby(TRANSMIT_INTERVAL_MINS * 60);
    }


    while (true);
}