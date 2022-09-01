#ifndef _CRAY_PROFILE_H_
#define _CRAY_PROFILE_H_

#include <float.h>
#include <inttypes.h>

typedef struct AveragedSample {
	int count;
	uint64_t totalTicks;
} AveragedSample;

typedef struct CycleProfile {
	double updatePlayerTimeMS;
	double totalRenderTimeMS;
	double clearTimeMS;
	AveragedSample tileRender;
	AveragedSample topRender;
	AveragedSample firstRender;
	AveragedSample vertRender;
} CycleProfile;

extern AveragedSample DefaultSample();
extern CycleProfile DefaultCycleProfile();
extern void AddSample(AveragedSample* const sample, uint64_t ticks);
extern void ResetProfile(CycleProfile* const profile);
extern void PrintAveragedSample(AveragedSample* const sample, const char* const name);
extern void PrintProfileStats(CycleProfile* const profile);

#endif // !_CRAY_PROFILE_H_
