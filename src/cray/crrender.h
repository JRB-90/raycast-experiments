#ifndef _CR_RENDER_H_
#define _CR_RENDER_H_

#include <stdbool.h>
#include "crscene.h"
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

// Private Test Functions

extern void RenderVerticalWallStrip(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const int xPosition,
    const int height,
    const double distanceToWall,
    const double angleWithWall,
    CycleProfile* profile
);
extern void RenderWallsTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
extern void RenderPlayerTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
extern void RenderProjectionTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
extern void RenderRayTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Vector2D* const ray,
    CycleProfile* profile
);

#endif // !_CR_RENDER_H_
