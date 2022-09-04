#ifndef _CR_INPUT_H_
#define _CR_INPUT_H_

#include <stdbool.h>

// Data types
typedef struct InputState {
	bool quit;
	bool forwards;
	bool backwards;
	bool rotRight;
	bool rotLeft;
	bool right;
	bool left;
	bool toggleDebug;
	bool toggleRenderMode;
} InputState;

// Functions definitions
extern InputState DefaultInputState();
extern void UpdateInputState(InputState* const state);

#endif // !_CR_INPUT_H_
