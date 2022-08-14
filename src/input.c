#include "input.h"
#include "SDL.h"

void UpdatePlayerControls(InputState* state, SDL_Event* e);

InputState CreateInputState()
{
    InputState inputState;
    inputState.quit = false;
    inputState.forwards = false;
    inputState.backwards = false;
    inputState.left = false;
    inputState.right = false;

    return inputState;
}

void UpdateInputState(InputState* state)
{
    SDL_Event e;

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

void UpdatePlayerControls(InputState* state, SDL_Event* e)
{
    if (e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_w:
            state->forwards = true;
            break;
        case SDLK_s:
            state->backwards = true;
            break;
        case SDLK_d:
            state->right = true;
            break;
        case SDLK_a:
            state->left = true;
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
            state->right = false;
            break;
        case SDLK_a:
            state->left = false;
            break;
        }
    }
}
