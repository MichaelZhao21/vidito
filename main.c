#define SDL_MAIN_USE_CALLBACKS 2 // Use callbacks instead of main
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* Run when app initializes */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Vidito", "1.0", "dev.mikz.vidito");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't init SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, 0,
                                   &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);

  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  /* SDL will clean up the window/renderer for us. */
}
