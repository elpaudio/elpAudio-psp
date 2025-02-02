#include "main.h"
#include "prepare.h"

winbox *window;
font *ea_font1;
font *ea_font2;
font *ea_font3;

sprite *sprOnOffline;
sprite *albumart;

int level=0;
int selx=0;
int sely=0;
int mypercent=0;
int mylastpercent=-1;

text *txt_elpAudio,
*txt_offline,*txt_online,
*txt_offlisten,*txt_onlisten,
*txt_nofiles,*txt_myfolder,
*txt_battery;

text *txt_title,*txt_artist,*txt_album;

extern "C" int main(int argc, char* args[]) {
    if(!prepare_everything()) exit(0x666);

    window=window_create(0,0,480,272);
    ea_font1=font_add("data/fonts/sourcesans.ttf",16);
    ea_font2=font_add("data/fonts/sourcesans.ttf",12);
    ea_font3=font_add("data/fonts/sourcesans.ttf",18);
    sprOnOffline=image_add("data/images/offonline.png",2,256,64);

    txt_elpAudio=new text("elpAudio", ea_font1, window->ren);
    txt_offline=new text("Offline",ea_font1,window->ren);
    txt_online=new text("Online",ea_font1,window->ren);
    txt_offlisten=new text("Offline listening",ea_font1,window->ren);
    txt_onlisten=new text("Online services",ea_font1,window->ren);
    txt_nofiles=new text("No files found!",ea_font1,window->ren);

    txt_title=new text("Untitled",ea_font1,window->ren);
    txt_battery=new text("100%",ea_font1,window->ren);
    txt_artist=new text("Unknown artist",ea_font2,window->ren);
    txt_album=new text("Unknown album",ea_font1,window->ren);
    txt_myfolder=new text("ms0:/MUSIC",ea_font3,window->ren);
    albumart=image_add("data/images/noalbum.png",1,96,96);

    while(window->running) {
        check_press();
        if(SDL_PollEvent(&window->ev)) {
            switch (window->ev.type) {
                case SDL_QUIT:
                    window->running=0;
                    break;

            }
        }
        /// should be here anytime
        SDL_SetRenderDrawBlendMode(window->ren,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(window->ren,80,80,80,255);
        SDL_RenderClear(window->ren);
        SDL_SetRenderDrawColor(window->ren,0,0,0,64);
        SDL_Rect myrect={0,0,480,32};
        SDL_RenderFillRect(window->ren,&myrect);
        myrect={0,272-32,480,32};
        SDL_RenderFillRect(window->ren,&myrect);
        SDL_SetRenderDrawColor(window->ren,255,255,255,255);
        draw_text(txt_elpAudio,5,5,0,0);
        SDL_GetPowerInfo(NULL,&mypercent);
        if(mylastpercent!=mypercent) {
            mylastpercent=mypercent;
            *txt_battery=(std::to_string(mypercent)+"%");
            //text_reprepare(txt_battery,(std::to_string(mypercent)+"%"));}
        }
        draw_text(txt_battery,5,245,0,0);
        /// end of "should be here anytime"

        ea_Level();
        SDL_RenderPresent(window->ren);
        SDL_Delay(1000.f/60.f);
    }
    delete[] window;
    delete[] ea_font1;
    delete[] ea_font2;
    delete[] ea_font3;
    return 0;
}
