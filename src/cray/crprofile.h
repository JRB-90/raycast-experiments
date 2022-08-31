#ifndef _CRAY_PROFILE_H_
#define _CRAY_PROFILE_H_

#include <float.h>

typedef struct CycleProfile {
	double totalRenderTimeMS;
	double clearTimeMS;
} CycleProfile;

extern CycleProfile DefaultCycleProfile();
extern void ResetProfile(CycleProfile* const profile);
extern void PrintProfileStats(CycleProfile* const profile);

#endif // !_CRAY_PROFILE_H_
