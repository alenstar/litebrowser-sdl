#include "container_sdl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

char master_css[] = {
#include "master.css.inc"
    , 0};

int main(int argc, char *argv[]) {
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bitmapTex = NULL;
    SDL_Surface *bitmapSurface = NULL;
    int posX = 100, posY = 100, width = 800, height = 600;

    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    bitmapSurface = SDL_LoadBMP("img/hello.bmp");
    bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
    SDL_FreeSurface(bitmapSurface);

    litehtml::context html_context;
    html_context.load_master_stylesheet(master_css);
    container_sdl *csdl = new container_sdl(&html_context, renderer);

    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_SPACE: {
                    std::string url = "file:///opt/workspace/beego/src/"
                                      "github.com/alenstar/litebrowser-sdl/"
                                      "build/index.html";
                    // std::string url = "http://www.litehtml.com/";
                    csdl->open_page(url);
                }
                case SDLK_ESCAPE:
                    break;
                }
            }
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        csdl->on_draw();
        // SDL_RenderClear(renderer);
        // SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
