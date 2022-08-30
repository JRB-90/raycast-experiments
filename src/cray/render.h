#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdbool.h>
#include "scene.h"
#include "display.h"

// Function defs
extern void RenderTiles(
	const Display* const display,
	const Scene* const scene,
	const DisplayTile tiles[],
	int count
);
extern void RenderTile(
	const Display* const display,
	const Scene* const scene,
	const DisplayTile* const tile
);
extern void RenderSceneTopDown(
	const Display* const display,
	const Scene* const scene
);
extern void RenderSceneFirstPerson(
	const Display* const display,
	const Scene* const scene
);
extern void ClearScreen(
	const Display* const display,
	const Scene* const scene
);

#endif // !_RENDER_H_
