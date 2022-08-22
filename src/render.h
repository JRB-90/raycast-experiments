#ifndef _RENDER_H_
#define _RENDER_H_

#include "scene.h"
#include "display.h"

// Function defs
extern void RenderTiles(
	const Display display,
	const Scene scene,
	const DisplayTile tiles[],
	int count
);
extern void RenderTile(
	const Display display, 
	const Scene scene,
	const DisplayTile tile 
);
extern void RenderSceneTopDown(
	const Display display, 
	const Scene scene
);
extern void RenderSceneFirstPerson(
	const Display display,
	const Scene scene
);
extern void ClearScreen(
	const Display display, 
	const Scene scene
);

#endif // !_RENDER_H_
