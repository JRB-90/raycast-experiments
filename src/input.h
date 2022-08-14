#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>

struct InputState {
	bool quit;
	bool forwards;
	bool backwards;
	bool right;
	bool left;
};

typedef struct InputState InputState;

extern InputState CreateInputState();
extern void UpdateInputState(InputState* state);

#endif // !_INPUT_H_
