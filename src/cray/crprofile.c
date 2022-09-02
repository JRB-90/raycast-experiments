#include "crprofile.h"

#include <stdio.h>
#include "crtime.h"

AveragedSample DefaultSample()
{
    AveragedSample sample =
    {
        .count = 0,
        .totalTicks = 0
    };

    return sample;
}

CycleProfile DefaultCycleProfile()
{
    CycleProfile profile =
    {
        .updatePlayerTimeMS = 0.0,
        .totalRenderTimeMS = 0.0,
        .renderPresentTimeMS = 0.0,
        .clearTimeMS = 0.0,
        .tileRender = DefaultSample(),
        .topRender = DefaultSample(),
        .firstRender = DefaultSample(),
        .vertRender = DefaultSample()
    };

    return profile;
}

void AddSample(AveragedSample* const sample, uint64_t ticks)
{
    sample->count++;
    sample->totalTicks += ticks;
}

void ResetProfile(CycleProfile* const profile)
{
    profile->updatePlayerTimeMS = 0.0;
    profile->totalRenderTimeMS = 0.0;
    profile->clearTimeMS = 0.0;
    profile->renderPresentTimeMS = 0.0;
    profile->tileRender = DefaultSample();
    profile->topRender = DefaultSample();
    profile->firstRender = DefaultSample();
    profile->vertRender = DefaultSample();
}

void PrintAveragedSample(AveragedSample* const sample, const char* const name)
{
    if (sample->count > 0)
    {
        double timeTotalMS = GetTimeInMS(sample->totalTicks);
        double timeAverageMS = timeTotalMS / (double)sample->count;

        printf("%s count:\t%i\n", name, sample->count);
        printf("Tot %s time:\t%f ms\n", name, timeTotalMS);
        printf("Ave %s time:\t%f ms\n", name, timeAverageMS);
    }
}

void PrintProfileStats(CycleProfile* const profile)
{
    printf("Up play time:\t%f ms\n", profile->updatePlayerTimeMS);
    printf("Tot ren time:\t%f ms\n", profile->totalRenderTimeMS);
    printf("Ren pres time:\t%f ms\n", profile->renderPresentTimeMS);
    printf("Clear time:\t%f ms\n", profile->clearTimeMS);
    PrintAveragedSample(&profile->tileRender, "Tile");
    PrintAveragedSample(&profile->topRender, "Top");
    PrintAveragedSample(&profile->firstRender, "First");
    PrintAveragedSample(&profile->vertRender, "Vert");
}
