#pragma once
#include "main.h"

extern winbox *window;
extern font *ea_font1,*ea_font2,*ea_font3;
extern sprite *sprOnOffline;

extern text *txt_elpAudio,
    *txt_online,*txt_offline,*txt_offlisten,*txt_onlisten,*txt_nofiles,*txt_myfolder;


extern int prepare_everything();

extern font *font_add(char const *fontfile,int size);
extern int draw_text(text *txt, int x, int y, float halign, float valign);
extern int draw_text_rt(font *fn, std::string txt, int x, int y, float halign, float valign);
extern winbox *window_create(int x,int y, int w, int h);
