#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdbool.h>
#include "crscene.h"
#include "crdisplay.h"
#include "crprofile.h"

// Function defs
extern void RenderTiles(
	const Display* const display,
	const Scene* const scene,
	const DisplayTile tiles[],
	int count,
	CycleProfile* const profile
);
extern void RenderTile(
	const Display* const display,
	const Scene* const scene,
	const DisplayTile* const tile,
	CycleProfile* const profile
);
extern void RenderSceneTopDown(
	const Display* const display,
	const Scene* const scene,
	CycleProfile* const profile
);
extern void RenderSceneFirstPerson(
	const Display* const display,
	const Scene* const scene,
	CycleProfile* const profile
);
extern void ClearScreen(
	const Display* const display,
	const Scene* const scene,
	CycleProfile* const profile
);

#endif // !_RENDER_H_
