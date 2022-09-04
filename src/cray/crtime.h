#ifndef _CR_TIME_H_
#define _CR_TIME_H_

#include <stdint.h>
#include <float.h>

extern uint64_t GetTicks();
extern double GetTimeInS(uint64_t elapsedTicks);
extern double GetTimeInMS(uint64_t elapsedTicks);
extern double GetTimeInUS(uint64_t elapsedTicks);

#endif // !_CR_TIME_H_
