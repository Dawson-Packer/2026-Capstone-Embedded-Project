#include "app.hpp"
#include "Modules/Filesystem/filesystem.hpp"
#include "Utils/logging.hpp"
#include <stdio.h>

extern "C" void App_Init(void)
{
    // printf(ANSI_BRIGHT_CYAN ANSI_BOLD "Application started." ANSI_RESET "\r\n");
    printf(ANSI_BRIGHT_CYAN ANSI_BOLD "\r\n");
    printf("oP\"Yb.  dP\"Yb  oP\"Yb.   dP'       88\"\"Yb 888888 8b    d8  dP\"Yb  888888 888888   "
           "  8b    d8  dP\"Yb  88b 88 88 888888  dP\"Yb  88\"\"Yb \r\n");
    printf("\"' dP' dP   Yb \"' dP' .d8'        88__dP 88__   88b  d88 dP   Yb   88   88__       "
           "88b  d88 dP   Yb 88Yb88 88   88   dP   Yb 88__dP  \r\n");
    printf(
        "  dP'  Yb   dP   dP'  8P\"\"\"Yb     88\"Yb  88\"\"   88YbdP88 Yb   dP   88   88\"\"     "
        "  88YbdP88 Yb   dP 88 Y88 88   88   Yb   dP 88\"Yb  \r\n");
    printf(".d8888  YbodP  .d8888 `YboodP     88  Yb 888888 88 YY 88  YbodP    88   888888     88 "
           "YY 88  YbodP  88  Y8 88   88    YbodP  88  Yb \r\n");
    printf(ANSI_RESET "\r\n");

    fs::Filesystem filesys = fs::Filesystem();
}
extern "C" void App_Run(void)
{

    while (1)
    {
    }
}