#include "logdef.h"
#include "container_sdl.h"


#ifdef __cplusplus
extern "C" {
#endif
#ifdef NANOVG_GLEW
#  include <GL/glew.h>
#endif
#include <GL/gl.h>
#ifdef __cplusplus
}
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "nanovg.h"
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"


char master_css[] = {
#include "master.css.inc"
    , 0};

int loadFonts(NVGcontext* vg)
{
    int i;

    if (vg == NULL)
        return -1;

    //data->images[i] = nvgCreateImage(vg, file, 0);

    int fontIcons = nvgCreateFont(vg, "icons", "../nanovg/example/entypo.ttf");
    if (fontIcons == -1) {
        LOGE("Could not add font icons.");
        return -1;
    }
    int fontNormal = nvgCreateFont(vg, "sans", "../nanovg/example/Roboto-Regular.ttf");
    if (fontNormal == -1) {
        LOGE("Could not add font italic.");
        return -1;
    }
    int fontBold = nvgCreateFont(vg, "sans-bold", "../nanovg/example/Roboto-Bold.ttf");
    if (fontBold == -1) {
        LOGE("Could not add font bold.");
        return -1;
    }
    int fontEmoji = nvgCreateFont(vg, "emoji", "DroidSansFallback.ttf");//"../nanovg/example/NotoEmoji-Regular.ttf");
    if (fontEmoji == -1) {
        LOGE("Could not add font emoji.");
        return -1;
    }
    nvgAddFallbackFontId(vg, fontNormal, fontEmoji);
    nvgAddFallbackFontId(vg, fontBold, fontEmoji);

    return 0;
}

int main(int argc, char *argv[]) {
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bitmapTex = NULL;
    SDL_Surface *bitmapSurface = NULL;
    int posX =  SDL_WINDOWPOS_UNDEFINED, posY =  SDL_WINDOWPOS_UNDEFINED, width = 800, height = 600;

    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Hello World", posX, posY, width, height, SDL_WINDOW_OPENGL);
    //renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG ); // May be a performance booster in *nix?
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
    SDL_GLContext glcontext = SDL_GL_CreateContext( win );
    // glEnable(GL_STENCIL_TEST);
  
  struct NVGcontext *vg = NULL;
  //struct NVGLUframebuffer *fb = NULL;
 #ifdef NANOVG_GLEW
    if(glewInit() != GLEW_OK) {
        LOGE("Could not init glew.");
        SDL_DestroyWindow(win);
        return -1;
    }
#endif

  LOGD( "Creating nanovg context" );
  vg = nvgCreateGLES2(NVG_ANTIALIAS|NVG_STENCIL_STROKES);
  if ( !vg ) {
    LOGE( "Couldn't create nanovg gl2 context" );
    // Couldn't create context
    SDL_DestroyWindow(win);
    return 2;
  } else {
    if (loadFonts(vg)!=0) {
        LOGE( "Couldn't create nanovg font context" );
        // Couldn't create context
        SDL_DestroyWindow(win);
        return 2;
    }
    }

    litehtml::context html_context;
    html_context.load_master_stylesheet(master_css);
    container_sdl *container = new container_sdl(&html_context, vg);
    bool running = true;
    while (running) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_SPACE: {
                    std::string url = argc == 1 ? "index.html" : argv[1]; // file:///index.html
                    // std::string url = "http://www.litehtml.com/";
                    container->open_page(url);
                    break;
                }
                case SDLK_ESCAPE:
                    running = false;
                    break;
                }
            }
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        // SDL_RenderClear(renderer);
        // container->on_draw(renderer);
        // // SDL_RenderClear(renderer);
        // // SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
        // SDL_RenderPresent(renderer);
        
        // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        nvgBeginFrame(vg, width, height, (float)width/(float)height);
        container->on_draw(vg);
        nvgEndFrame(vg);

        glEnable(GL_DEPTH_TEST);
        SDL_GL_SwapWindow( win );
    }
    nvgDeleteGLES2( vg );
    SDL_GL_DeleteContext(glcontext);  
    // SDL_DestroyTexture(bitmapTex);
    // SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
