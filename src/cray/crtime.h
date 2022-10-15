#ifndef _CR_TIME_H_
#define _CR_TIME_H_

#include <stdint.h>
#include <float.h>

extern uint64_t GetTicks();
extern float GetTimeInS(uint64_t elapsedTicks);
extern float GetTimeInMS(uint64_t elapsedTicks);
extern float GetTimeInUS(uint64_t elapsedTicks);

#endif // !_CR_TIME_H_
