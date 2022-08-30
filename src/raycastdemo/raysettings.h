#ifndef _CRAY_SETTINGS_H_

#include <stdbool.h>

typedef enum RenderMode {
	FullStaticScene,
	FullFirstPerson,
	Tiled
} RenderMode;

typedef struct RaycastSettings {
	bool printDebugInfo;
	RenderMode renderMode;
} RaycastSettings;

#endif // !_CRAY_SETTINGS_H_
