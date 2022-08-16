#ifndef _RENDER_H_
#define _RENDER_H_

#include "scene.h"
#include "display.h"

// Function defs
extern void RenderScene(const Display display, const Scene* const scene);
extern void ClearScreen(const Display display, Color color);

#endif // !_RENDER_H_
