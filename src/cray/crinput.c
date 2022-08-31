#include "crinput.h"
#include "SDL.h"

void ResetPulseKeys(InputState* const state);
void UpdatePlayerControls(InputState* const state, SDL_Event* e);

InputState DefaultInputState()
{
    InputState inputState =
    {
        .quit = false,
        .forwards = false,
        .backwards = false,
        .rotLeft = false,
        .rotRight = false,
        .left = false,
        .right = false,
        .toggleDebug = false,
        .toggleRenderMode = false,
    };

    return inputState;
}

void UpdateInputState(InputState* const state)
{
    SDL_Event e;
    ResetPulseKeys(state);

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            state->quit = true;

            return;
        }
        else
        {
            UpdatePlayerControls(state, &e);
        }
    }
}

void ResetPulseKeys(InputState* const state)
{
    state->toggleDebug = false;
    state->toggleRenderMode = false;
}

void UpdatePlayerControls(InputState* const state, SDL_Event* e)
{
    if (e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_ESCAPE:
            state->quit = true;
            return;
        case SDLK_v:
            state->toggleDebug = true;
            break;
        case SDLK_b:
            state->toggleRenderMode = true;
            break;
        case SDLK_w:
            state->forwards = true;
            break;
        case SDLK_s:
            state->backwards = true;
            break;
        case SDLK_d:
            if (e->key.keysym.mod == KMOD_LCTRL)
            {
                state->rotRight = false;
                state->right = true;
            }
            else
            {
                state->rotRight = true;
                state->right = false;
            }
            break;
        case SDLK_a:
            if (e->key.keysym.mod == KMOD_LCTRL)
            {
                state->rotLeft = false;
                state->left = true;
            }
            else
            {
                state->rotLeft = true;
                state->left = false;
            }
            break;
        }
    }
    else if (e->type == SDL_KEYUP)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_w:
            state->forwards = false;
            break;
        case SDLK_s:
            state->backwards = false;
            break;
        case SDLK_d:
            state->rotRight = false;
            state->right = false;
            break;
        case SDLK_a:
            state->rotLeft = false;
            state->left = false;
            break;
        }
    }
}
