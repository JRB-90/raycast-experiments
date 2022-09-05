#ifndef _CR_RENDER_H_
#define _CR_RENDER_H_

#include <stdbool.h>
#include "crscene.h"
#include "crdisplay.h"
#include "crprofile.h"

// Function defs
extern void RenderTiles(
	const ScreenBuffer* const screen,
	const Scene* const scene,
	const DisplayTile tiles[],
	int count,
	CycleProfile* const profile
);
extern void RenderTile(
	const ScreenBuffer* const screen,
	const Scene* const scene,
	const DisplayTile* const tile,
	CycleProfile* const profile
);
extern void RenderSceneTopDown(
	const ScreenBuffer* const screen,
	const Scene* const scene,
	CycleProfile* const profile
);
extern void RenderSceneFirstPerson(
	const ScreenBuffer* const screen,
	const Scene* const scene,
	CycleProfile* const profile
);
extern void ClearScreen(
	const ScreenBuffer* const screen,
	const Scene* const scene,
	CycleProfile* const profile
);

#endif // !_CR_RENDER_H_
