#include "images.h"
#include <SDL2/SDL_stdinc.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

sprite *image_add(std::string path, int frames, int myw, int myh) {
    sprite *spr=new sprite;
    spr->frames=frames;
    char running = 0;
    SDL_Event event;
    uint8_t * image_data = NULL;
    uint8_t * new_image_data = NULL;
    int image_width, image_height, image_pixel_size;
    SDL_PixelFormatEnum format_sdl;
    stbir_pixel_layout format_stbir;
    image_data = stbi_load(path.c_str(), &image_width, &image_height, &image_pixel_size, STBI_default);
    switch (image_pixel_size)
    {
        case STBI_grey:
        stbi_image_free(image_data);
        image_data = stbi_load(path.c_str(), &image_width, &image_height, &image_pixel_size, STBI_rgb);
        format_sdl = SDL_PIXELFORMAT_RGB24;
        format_stbir = STBIR_RGB;
        break;
        case STBI_grey_alpha:
        stbi_image_free(image_data);
        image_data = stbi_load(path.c_str(), &image_width, &image_height, &image_pixel_size, STBI_rgb_alpha);
        format_sdl = SDL_PIXELFORMAT_RGBA32;
        format_stbir = STBIR_RGBA;
        break;
        case STBI_rgb:
        format_sdl = SDL_PIXELFORMAT_RGB24;
        format_stbir = STBIR_RGB;
        break;
        case STBI_rgb_alpha:
        format_sdl = SDL_PIXELFORMAT_RGBA32;
        format_stbir = STBIR_RGBA;
        break;
        default:
        return nullptr;
        break;
    }
    spr->surf = SDL_CreateRGBSurfaceWithFormat(0, myw, myh, 0, format_sdl);
    stbir_resize_uint8_linear(image_data, image_width, image_height, 0, (Uint8 *)spr->surf->pixels, spr->surf->w, spr->surf->h, 0, format_stbir);
    stbi_image_free(image_data);
    image_data = NULL;

    spr->tex = SDL_CreateTextureFromSurface(window->ren, spr->surf);
    spr->w=spr->surf->w/fmax(frames,1);
    spr->h=spr->surf->h;
    for(int i=0;i<frames;i++) {
        spr->frame[i]={spr->w*i,0,spr->w*(i+1),spr->h};
    }
    SDL_FreeSurface(spr->surf);
    return spr;
}

void image_draw(sprite *spr,int frame,int x, int y, float xscale, float yscale, SDL_Color col,float rot) {
    SDL_FRect myrect={(float)x,(float)y,spr->w*xscale,spr->h*yscale};
    SDL_FPoint mypoint={0,0};
    SDL_SetTextureColorMod(spr->tex,col.r,col.g,col.b);
    SDL_RenderCopyExF(window->ren,spr->tex,&spr->frame[frame%spr->frames],&myrect,rot,&mypoint,SDL_RendererFlip::SDL_FLIP_NONE);
}
void image_delete(sprite *spr) {
    if(spr->surf!=nullptr) {SDL_FreeSurface(spr->surf);spr->surf=nullptr;}
    if(spr->tex!=nullptr) {SDL_DestroyTexture(spr->tex);spr->tex=nullptr;}
    spr->frames=0;
    spr->w=0;
    spr->h=0;
}
