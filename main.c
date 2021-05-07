#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "./game.h"

#include <SDL.h>

void sdl_check_code(int code)
{
    if (code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

void *sdl_check_ptr(void *ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}

int main(void)
{
    init();
    const size_t display_width = get_display_width();
    const size_t display_height = get_display_height();
    uint32_t *display_pixels = get_display();
    const SDL_Rect display_rect = {
        0, 0, display_width, display_height
    };

    sdl_check_code(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window * const window =
        sdl_check_ptr(
            SDL_CreateWindow(
                "rust-browser-game-but-sdl",
                0, 0,
                display_width,
                display_height,
                SDL_WINDOW_RESIZABLE));

    SDL_Renderer * const renderer =
        sdl_check_ptr(
            SDL_CreateRenderer(
                window, -1, SDL_RENDERER_ACCELERATED));

    sdl_check_code(
        SDL_RenderSetLogicalSize(renderer, display_width, display_height));

    SDL_Texture * display_texture =
        sdl_check_ptr(
            SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_ABGR8888,
                SDL_TEXTUREACCESS_STREAMING,
                display_width,
                display_height));

    bool quit = false;
    while (!quit) {
        SDL_Event event = {0};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                quit = true;
            }
            break;

            case SDL_MOUSEMOTION: {
                mouse_move(event.motion.x, event.motion.y);
            }
            break;

            case SDL_MOUSEBUTTONDOWN: {
                mouse_click(event.button.x, event.button.y);
            }
            break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    toggle_pause();
                }
            }
            break;
            }
        }

        next_frame(1.0f / 60.0f);

        {

            void *pixels = NULL;
            int pitch = 0;

            sdl_check_code(
                SDL_LockTexture(display_texture, &display_rect, &pixels, &pitch));

            for (size_t y = 0; y < display_height; ++y) {
                memcpy(
                    (char*) pixels + y * pitch,
                    display_pixels + y * display_width,
                    display_width * 4);
            }

            SDL_UnlockTexture(display_texture);
        }

        sdl_check_code(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
        sdl_check_code(SDL_RenderClear(renderer));

        sdl_check_code(
            SDL_RenderCopy(renderer,
                           display_texture,
                           &display_rect,
                           &display_rect));

        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
    }

    SDL_Quit();

    return 0;
}
