#include "crtime.h"

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

float GetTimeInS(uint64_t elapsedTicks)
{
    LARGE_INTEGER f;
    if (QueryPerformanceFrequency(&f))
    {
        return (float)elapsedTicks / (float)f.QuadPart;
    }
    else
    {
        return 0.0f;
    }
}

float GetTimeInMS(uint64_t elapsedTicks)
{
    LARGE_INTEGER f;
    if (QueryPerformanceFrequency(&f))
    {
        return (float)elapsedTicks / ((float)f.QuadPart / 1000.0f);
    }
    else
    {
        return 0.0f;
    }
}

float GetTimeInUS(uint64_t elapsedTicks)
{
    LARGE_INTEGER f;
    if (QueryPerformanceFrequency(&f))
    {
        return (float)elapsedTicks / ((float)f.QuadPart / 1000000.0f);
    }
    else
    {
        return 0.0f;
    }
}
#endif // _WIN32

#ifdef __unix
#include "time.h"

uint64_t GetTicks()
{
    struct timespec t;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);

    return t.tv_nsec;
}

float GetTimeInS(uint64_t elapsedTicks)
{
    return (float)((float)elapsedTicks / 1000000000.0f);
}

float GetTimeInMS(uint64_t elapsedTicks)
{
    return (float)((float)elapsedTicks / 1000000.0f);
}

float GetTimeInUS(uint64_t elapsedTicks)
{
    return (float)((float)elapsedTicks / 1000.0f);
}
#endif // __unix
