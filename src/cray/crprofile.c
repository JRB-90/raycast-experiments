#include "crprofile.h"

#include <stdio.h>

CycleProfile DefaultCycleProfile()
{
    CycleProfile profile =
    {
        .totalRenderTimeMS = 0.0,
        .clearTimeMS = 0.0
    };

    return profile;
}

void ResetProfile(CycleProfile* const profile)
{
    profile->totalRenderTimeMS = 0.0;
    profile->clearTimeMS = 0.0;
}

void PrintProfileStats(CycleProfile* const profile)
{
    printf("Total time: %f ms\n", profile->totalRenderTimeMS);
    printf("Clear time: %f ms\n", profile->clearTimeMS);
}
