#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <curl/curl.h>
#include <string>
#include <vector>

typedef struct winbox {
    SDL_Window *win;
    SDL_Renderer *ren;
    float maxfps;
    int running;
    SDL_Event ev;
    SDL_GameController *joy;
} winbox;

typedef struct font {
    TTF_Font *font;
    int size;
    int w,h;
} font;

#define MAX_BUTTONS SDL_CONTROLLER_BUTTON_MAX
typedef struct control {
    int butpress[MAX_BUTTONS];
    int lastbutpress[MAX_BUTTONS];
} control;

typedef struct sprite {
    SDL_Surface *surf;
    SDL_Texture *tex;
    SDL_Rect frame[256];
    int w,h,
    frames;
} sprite;

typedef struct music {
    std::string artist,name,album,date,number,fname;
    Mix_Music *mus;
    int len,loops;
    double pos;
} music;
extern music *ea_music;

/*
typedef struct _text {
    SDL_Surface *surf;
    SDL_Texture *tex;
    std::string txt;
} text;
*/


struct text
{
public:
  text(const std::string& newText, font *fn, SDL_Renderer* ren) :
    m_font(fn),
    m_ren(ren)
  {
    *this = newText;
  }

  ~text()
  {
    if(this->surf != nullptr) SDL_FreeSurface(this->surf);
    if(this->tex  != nullptr) SDL_DestroyTexture(this->tex);
  }

  text& operator=(const std::string& newText)
  {
    this->txt = newText;

    if(this->surf != nullptr) SDL_FreeSurface(this->surf);
    if(this->tex  != nullptr) SDL_DestroyTexture(this->tex);

    this->surf = TTF_RenderUTF8_Blended(m_font->font, this->txt.c_str(),(SDL_Color){255,255,255,255});
    this->tex  = SDL_CreateTextureFromSurface(m_ren, this->surf);

    return *this;
  }

  std::string txt{ };
  SDL_Surface* surf{ nullptr };
  SDL_Texture* tex{ nullptr };

private:
  font* m_font{ nullptr };
  SDL_Renderer* m_ren{ nullptr };
};


extern control *buttons;
extern sprite *albumart;

extern int level,selx,sely;
extern text *txt_title,*txt_artist,*txt_album;

#include "maths.h"
#include "sounds.h"
#include "controls.h"
#include "images.h"
#include "prepare.h"
#include "https_api.h"

#include "levels.h"
