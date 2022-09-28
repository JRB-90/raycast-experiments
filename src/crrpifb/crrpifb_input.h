#ifndef _CR_INPUT_H_
#define _CR_INPUT_H_

#include "crtypes.h"

// Functions definitions
extern int InitInputDevice();
extern int DestroyInputDevice();
extern int UpdateInputState(InputState* const state);

#endif // !_CR_INPUT_H_
