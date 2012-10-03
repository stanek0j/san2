
#pragma once

#ifdef LINUX
    #include <unistd.h>
#endif

#ifdef WINDOWS
    #include <windows.h> // Tenhle header dela problemy kvuli WinSocku (redefinition)
#endif

namespace San2
{
    namespace Utils
    {
        // Trik je v tom, ze kdyz nebude definovano ani LINUX ani WINDOWS, pak zahlasi uz komilator chybu
        // a ne az potom linker
        #ifdef LINUX
            void SanSleep(unsigned int seconds);
        #endif

        #ifdef WINDOWS
            void SanSleep(unsigned int seconds);
        #endif
    }
}
