#ifndef _CR_PROFILE_H_
#define _CR_PROFILE_H_

#include <float.h>
#include <inttypes.h>

typedef struct AveragedSample {
	int count;
	uint64_t totalTicks;
} AveragedSample;

typedef struct CycleProfile {
	float updatePlayerTimeMS;
	float totalRenderTimeMS;
	float clearTimeMS;
	float renderPresentTimeMS;
	AveragedSample tileRender;
	AveragedSample topRender;
	AveragedSample firstRender;
	AveragedSample vertRender;
} CycleProfile;

extern AveragedSample DefaultSample();
extern CycleProfile DefaultCycleProfile();
extern void AddSample(AveragedSample* const sample, uint64_t ticks);
extern void ResetProfile(CycleProfile* const profile);
extern void PrintAveragedSample(const AveragedSample* const sample, const char* const name);
extern void PrintProfileStats(const CycleProfile* const profile);

#endif // !_CR_PROFILE_H_
