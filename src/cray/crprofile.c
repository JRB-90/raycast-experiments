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
        .updatePlayerTimeMS = 0.0f,
        .totalRenderTimeMS = 0.0f,
        .renderPresentTimeMS = 0.0f,
        .clearTimeMS = 0.0f,
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
    profile->updatePlayerTimeMS = 0.0f;
    profile->totalRenderTimeMS = 0.0f;
    profile->clearTimeMS = 0.0f;
    profile->renderPresentTimeMS = 0.0f;
    profile->tileRender = DefaultSample();
    profile->topRender = DefaultSample();
    profile->firstRender = DefaultSample();
    profile->vertRender = DefaultSample();
}

void PrintAveragedSample(const AveragedSample* const sample, const char* const name)
{
    if (sample->count > 0)
    {
        float timeTotalMS = GetTimeInMS(sample->totalTicks);
        float timeAverageMS = timeTotalMS / (float)sample->count;

        printf("%s count:\t%i\n", name, sample->count);
        printf("Tot %s time:\t%f ms\n", name, timeTotalMS);
        printf("Ave %s time:\t%f ms\n", name, timeAverageMS);
    }
}

void PrintProfileStats(const CycleProfile* const profile)
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
