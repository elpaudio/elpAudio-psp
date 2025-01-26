#include "prepare.h"
#include "controls.h"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

// thanks to https://blog.rubenwardy.com/2023/01/24/using_sdl_gamecontroller/
SDL_GameController *findController() {
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            return SDL_GameControllerOpen(i);
        }
    }

    return nullptr;
}

font *font_add(char const *fontfile,int size) {
    font *fn=new font;
    fn->font=TTF_OpenFont(fontfile, size);
    fn->size=size;
    return fn;
}

int draw_text_rt(font *fn, std::string txt, int x, int y, float halign, float valign) {
    SDL_Surface *surf=TTF_RenderUTF8_Blended(fn->font,txt.c_str(),(SDL_Color){255,255,255,255});
    SDL_Texture *tex=SDL_CreateTextureFromSurface(window->ren,surf);
    SDL_Rect myrect={0,0,surf->w,surf->h};
    SDL_FRect extrect={x-halign*surf->w,y-valign*surf->h,(float)surf->w,(float)surf->h};
    SDL_RenderCopyF(window->ren,tex,&myrect,&extrect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
    return 1;
}

int draw_text(text *txt, int x, int y, float halign, float valign) {
    SDL_Rect myrect={0,0,txt->surf->w,txt->surf->h};
    SDL_FRect extrect={x-halign*txt->surf->w,y-valign*txt->surf->h,(float)txt->surf->w,(float)txt->surf->h};
    SDL_RenderCopyF(window->ren,txt->tex,&myrect,&extrect);
    return 1;
}

winbox *window_create(int x,int y, int w, int h) {
    winbox *win=new winbox;
    if(win==nullptr) return 0;
    win->win=SDL_CreateWindow("",x,y,w,h,SDL_WINDOW_SHOWN);
    win->ren=SDL_CreateRenderer(win->win,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
    win->running=1;
    win->joy=findController();
    SDL_GameControllerEventState(SDL_ENABLE);
    return win;
}

control *buttons;

int prepare_everything() {
    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)<0) return 0;
    if(Mix_Init(MIX_INIT_MID|MIX_INIT_FLAC|MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD)==0) return 0;
    if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF)==0)return 0;
    if(TTF_Init()<0) return 0;

    int freq=44100,channels=2;
    Uint16 format=AUDIO_U16;
    Mix_QuerySpec(&freq,&format,&channels);
    Mix_OpenAudio(freq,format,channels,4096);

    prepare_system_sounds();
    buttons=new control;
    reset_press();

    return 1;
}
