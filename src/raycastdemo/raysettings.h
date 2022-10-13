#ifndef _CRAY_SETTINGS_H_

#include <stdbool.h>
#include <crtypes.h>
#include "crscene.h"

typedef enum RenderMode {
	FullStaticScene,
	FullFirstPerson,
	Tiled
} RenderMode;

typedef struct RaycastSettings {
	bool printDebugInfo;
	RenderMode renderMode;
	ScreenFormat screenFormat;
	unsigned int targetFps;
	PlayerSettings playerSettings;
} RaycastSettings;

RaycastSettings ParseCommandLine(
	int argc,
	char* argv[]
);

#endif // !_CRAY_SETTINGS_H_
