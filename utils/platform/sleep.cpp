
#include "sleep.hpp"

namespace San2
{
    namespace Utils
    {
        #ifdef LINUX
            void sleep(unsigned int seconds)
            {
                sleep(seconds);
            }
        #endif

        #ifdef WINDOWS
            void sleep(unsigned int seconds)
            {
                // Ano, jenom takhle je to spravne SleepEx a TRUE. Jinak to zablokuje vsechny thready
                // a to je potom prusvih

                /*
                    The return value is WAIT_IO_COMPLETION if the function returned due to one or 
                    more I/O completion callback functions. This can happen only if bAlertable is TRUE, 
                    and if the thread that called the SleepEx function is the same thread that called 
                    the extended I/O function.

                    Strucne receno musi to byt TRUE, jinak prestane fungovat minimalne Cppl.

                    http://msdn.microsoft.com/en-us/library/windows/desktop/ms686307(v=vs.85).aspx
                */
                SleepEx(seconds * 1000, TRUE);
            }
        #endif
    }
}