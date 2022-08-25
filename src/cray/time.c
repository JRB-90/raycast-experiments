#include "time.h"

#ifdef _WIN32
#include <windows.h>

uint64_t GetTicks()
{
    uint64_t ticks = 0;

    LARGE_INTEGER t;
    if (QueryPerformanceCounter(&t))
    {
        return t.QuadPart;
    }
    else
    {
        return 0;
    }
}

double GetTimeInS(uint64_t elapsedTicks)
{
    LARGE_INTEGER f;
    if (QueryPerformanceFrequency(&f))
    {
        return (double)elapsedTicks / (double)f.QuadPart;
    }
    else
    {
        return 0.0;
    }
}

double GetTimeInMS(uint64_t elapsedTicks)
{
    LARGE_INTEGER f;
    if (QueryPerformanceFrequency(&f))
    {
        return (double)elapsedTicks / ((double)f.QuadPart / 1000.0);
    }
    else
    {
        return 0.0;
    }
}

double GetTimeInUS(uint64_t elapsedTicks)
{
    LARGE_INTEGER f;
    if (QueryPerformanceFrequency(&f))
    {
        return (double)elapsedTicks / ((double)f.QuadPart / 1000000.0);
    }
    else
    {
        return 0.0;
    }
}
#endif // _WIN32

#ifdef __unix
// TODO
#endif // __unix
