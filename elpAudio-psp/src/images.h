#pragma once
#include "main.h"

extern sprite *image_add(std::string path, int frames, int myw, int myh);

extern void image_draw(sprite *spr,int frame,int x, int y, float xscale, float yscale, SDL_Color col,float rot);

extern void image_delete(sprite *spr);
