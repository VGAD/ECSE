#include "Common.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace ECSE
{

void errorMessage(std::string message)
{
#ifdef _WIN32
    MessageBoxA(NULL, message.c_str(),  "Error!", MB_ICONEXCLAMATION | MB_OK);
#else
    // Oh dear! No code here yet.
#endif
}

}
