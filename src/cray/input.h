#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>

// Data types
typedef struct InputState {
	bool quit;
	bool forwards;
	bool backwards;
	bool right;
	bool left;
} InputState;

// Functions definitions
extern InputState DefaultInputState();
extern void UpdateInputState(InputState* const state);

#endif // !_INPUT_H_
