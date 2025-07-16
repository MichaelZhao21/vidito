#define SDL_MAIN_USE_CALLBACKS 2 // Use callbacks instead of main
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

#define CLAY_IMPLEMENTATION
#include "lib/clay.h"
#include "lib/clay_renderer_SDL3.c"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_TextEngine *textEngine = NULL;
static TTF_Font **fonts = NULL;

/* Handle Clay Errors */
void HandleClayErrors(Clay_ErrorData errorData) {
  printf("%s", errorData.errorText.chars);
}

static const Clay_Color COLOR_LIGHT = (Clay_Color){224, 215, 210, 255};
static const Clay_Color COLOR_BLACK = (Clay_Color){0, 0, 0, 255};

static inline Clay_Dimensions SDL_MeasureText(Clay_StringSlice text,
                                              Clay_TextElementConfig *config,
                                              void *userData) {
  TTF_Font **fonts = userData;
  TTF_Font *font = fonts[config->fontId];
  int width, height;

  TTF_SetFontSize(font, config->fontSize);
  if (!TTF_GetStringSize(font, text.chars, text.length, &width, &height)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to measure text: %s",
                 SDL_GetError());
  }

  return (Clay_Dimensions){(float)width, (float)height};
}

/* Create sample layout */
Clay_RenderCommandArray create_layout() {
  Clay_BeginLayout();

  Clay_Sizing layoutExpand = {.width = CLAY_SIZING_GROW(0),
                              .height = CLAY_SIZING_GROW(0)};

  CLAY({.id = CLAY_ID("container"),
        .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM,
                   .sizing = layoutExpand,
                   .padding = CLAY_PADDING_ALL(16),
                   .childGap = 16},
        .backgroundColor = COLOR_LIGHT}) {
    CLAY_TEXT(CLAY_STRING("hewwo worwld"),
              CLAY_TEXT_CONFIG({.fontSize = 16, .textColor = COLOR_BLACK}));
  }

  return Clay_EndLayout();
}

/* Run when app initializes */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  if (!TTF_Init()) {
    return SDL_APP_FAILURE;
  }

  SDL_SetAppMetadata("Vidito", "1.0", "dev.mikz.vidito");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't init SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Example Window", 640, 480, 0, &window,
                                   &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetWindowResizable(window, true);

  textEngine = TTF_CreateRendererTextEngine(renderer);
  if (!textEngine) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Failed to create text engine from renderer: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  fonts = SDL_calloc(1, sizeof(TTF_Font *));
  if (!fonts) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Failed to allocate memory for the font array: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  TTF_Font *font = TTF_OpenFont("Roboto-Regular.ttf", 24);
  if (!font) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  fonts[0] = font;

  /* Initialize Clay */
  uint64_t totalMem = Clay_MinMemorySize();
  Clay_Arena clayMem =
      (Clay_Arena){.memory = SDL_malloc(totalMem), .capacity = totalMem};

  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  Clay_Initialize(clayMem, (Clay_Dimensions){(float)width, (float)height},
                  (Clay_ErrorHandler){HandleClayErrors});
  Clay_SetMeasureTextFunction(SDL_MeasureText, fonts);

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  SDL_AppResult ret_val = SDL_APP_CONTINUE;

  switch (event->type) {
  case SDL_EVENT_QUIT:
    ret_val = SDL_APP_SUCCESS;
    break;
  case SDL_EVENT_WINDOW_RESIZED:
    Clay_SetLayoutDimensions((Clay_Dimensions){(float)event->window.data1,
                                               (float)event->window.data2});
    break;
  case SDL_EVENT_MOUSE_MOTION:
    Clay_SetPointerState((Clay_Vector2){event->motion.x, event->motion.y},
                         event->motion.state & SDL_BUTTON_LMASK);
    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    Clay_SetPointerState((Clay_Vector2){event->button.x, event->button.y},
                         event->button.button == SDL_BUTTON_LEFT);
    break;
  case SDL_EVENT_MOUSE_WHEEL:
    Clay_UpdateScrollContainers(
        true, (Clay_Vector2){event->wheel.x, event->wheel.y}, 0.01f);
    break;
  default:
    break;
  };

  return ret_val;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  Clay_RenderCommandArray render_commands = create_layout();

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  Clay_SDL3RendererData *data = SDL_calloc(1, sizeof(Clay_SDL3RendererData));
  data->renderer = renderer;
  data->fonts = fonts;
  data->textEngine = textEngine;
  SDL_Clay_RenderClayCommands(data, &render_commands);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  /* SDL will clean up the window/renderer for us. */
}
