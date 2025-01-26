#include "levels.h"
#include "controls.h"
#include "images.h"
#include "main.h"
#include "prepare.h"
#include "sounds.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <dirent.h>
#include <filesystem>
#include <set>

namespace fs=std::filesystem;

std::string supported_files=".mp3;.wav;.flac;.ogg;.wma;.aac;.mod;.xm;.s3m;.it";

std::string mydir="ms0:/MUSIC";
std::string mylastdir="";
std::string lastfile="";

#define MAX_FILES_SHOWN 12

std::vector<int> myfiletype;
std::vector<text *> myfileren;
std::vector<std::string> myfile;

int myfiles=0;

int update=0;
SDL_Rect posrect={0,0,0,0};

void start_search(std::string dir) {
    if(mylastdir!=dir) {
        *txt_myfolder=dir;
        myfiles=0;
        struct dirent *de1;
        DIR *d1=opendir(dir.c_str());
        while((de1=readdir(d1))!=nullptr) myfiles++;
        closedir(d1);
        delete[] de1;
        DIR *d2=opendir(dir.c_str());
        struct dirent *de2;
        int i=0;
        myfile.clear();
        myfileren.clear();
        myfiletype.clear();
        myfile.resize(myfiles+1);
        myfileren.resize(myfiles+1);
        myfiletype.resize(myfiles+1);
        while((de2=readdir(d2))!=nullptr) {
            fs::path mypath=de2->d_name;
            if(
                (supported_files.find_first_of((mypath).extension())!=std::string::npos||de2->d_type==DT_DIR)
                &&(mypath.compare(".")!=0&&mypath.compare("..")!=0)
                &&((std::set<std::string>{".jpg", ".JPG", ".jpeg",".JPEG"}).count((mypath).extension())==0))
            {
                myfile[i].append(de2->d_name);
                myfileren[i]=new text(myfile[i],ea_font2,window->ren);
                myfiletype[i]=de2->d_type;
                i++;
            }
        }
        myfiles=i;
        closedir(d2);
        delete[] de2;
        mylastdir=dir;
    }
}

void ea_Level() {
    switch(level) {
        case 0: {
            if(check_button_pressed(but_controls::dpad_right) or check_button_pressed(but_controls::dpad_left)) {selx=!selx; playSelectSound();}
            if(check_button_pressed(but_controls::x)) {if(selx==0){level=20;}else level=10; playPressSound(); selx=0; sely=0;}
            SDL_SetRenderDrawColor(window->ren,0,0,0,80+selx*50);
            SDL_Rect myrect={60,68,120,136};
            SDL_RenderFillRect(window->ren,&myrect);
            SDL_SetRenderDrawColor(window->ren,0,0,0,80+(1-selx)*50);
            myrect=myrect={300,68,120,136};
            SDL_RenderFillRect(window->ren,&myrect);
            draw_text(txt_online,120,136,0.5,-0.5);
            draw_text(txt_offline,360,136,0.5,-0.5);
            image_draw(sprOnOffline,0,60,68,120.f/128,68.f/64,(SDL_Color){(Uint8)(128+128*selx),(Uint8)(128+128*selx),(Uint8)(128+128*selx)},0);
            image_draw(sprOnOffline,1,300,68,120.f/128,68.f/64,(SDL_Color){(Uint8)(128+128*(1-selx)),(Uint8)(128+128*(1-selx)),(Uint8)(128+128*(1-selx))},0);
            break;
        }
        /// OFFLINE LISTENING!!!!!!!!!!!!!!
        case 10: {
            if(check_button_pressed(but_controls::o)) {level=0; playBackSound();}
            if(check_button_pressed(but_controls::dpad_down)) {if (selx<myfiles-1)selx++; else selx=0;}
            if(check_button_pressed(but_controls::dpad_up)) {if (selx>0)selx--; else selx=myfiles-1;}
            if(check_button_pressed(but_controls::dpad_up)||check_button_pressed(but_controls::dpad_down)) {if(ea_music->mus==nullptr) playSelectSound();}
            if(check_button_pressed(but_controls::x)) {
                switch(myfiletype[selx]) {
                    case DT_REG: {
                        if(lastfile.compare(mydir+"/"+myfile[selx])!=0) {
                            ea_music_open(mydir+"/"+myfile[selx]);
                            ea_music_play();
                            ea_music_get_metadata();
                        }
                        /*if(albumart!=nullptr) image_delete(albumart);

                        if(fs::exists(mydir+"/COVER.JPG")) albumart=image_add(mydir+"/COVER.JPG",1,90,90);
                        else if(fs::exists(mydir+"/FRONT.JPG")) albumart=image_add(mydir+"/FRONT.JPG",1,90,90);
                        else albumart=image_add("data/images/noalbum.png",1,90,90);*/
                        level=11;
                        break;
                    }
                    case DT_DIR: {mydir+="/"+myfile[selx]; selx=0;break;}
                    default:break;
                }
            }
            if(check_button_pressed(but_controls::d)&&mydir!="ms0:/MUSIC") {
                std::filesystem::path mypath=mydir;
                mydir=mypath.parent_path();
                selx=0;
            }
            int scroll=clamp(selx-(double)MAX_FILES_SHOWN/2+1,0,fmax(myfiles-1-MAX_FILES_SHOWN/2,0));
            draw_text(txt_offlisten,475,5,1,0);
            start_search(mydir);
            draw_text(txt_myfolder,128,32,0,0);
            if(myfiles==0) draw_text(txt_nofiles,240,136,0.5,0.5);
            else {
                for(int i=0;i<clamp(myfiles,0,MAX_FILES_SHOWN);i++) {
                    SDL_Rect myrect={127,64+i*14,353,14};
                    if(selx==i+scroll) {
                        SDL_SetRenderDrawColor(window->ren,0,0,0,80);
                        SDL_RenderFillRect(window->ren,&myrect);
                        SDL_SetRenderDrawColor(window->ren,255,255,255,255);
                    }
                    if(i+scroll<myfiles)draw_text(myfileren[i+scroll],128,64+i*14,0,0);
                }
            }
            break;
        }
        case 11: {
            if(check_button_pressed(but_controls::o)) {level=10;}
            if(check_button_pressed(but_controls::r)) {if (selx<myfiles-1)selx++; else selx=0; ea_music_play_next(mydir+"/"+myfile[selx]);}
            if(check_button_pressed(but_controls::l)) {if (selx>0)selx--; else selx=myfiles-1; ea_music_play_next(mydir+"/"+myfile[selx]);}
            if(check_button_pressed(but_controls::dpad_left)||check_button_pressed(but_controls::dpad_right)) ea_music_pause();
            if(check_button_released(but_controls::dpad_left)||check_button_released(but_controls::dpad_right)) ea_music_resume();
            if(check_button_pressed(but_controls::x)) {
                if(!ea_music_paused()) ea_music_pause();
                else ea_music_resume();
            }
            double mypos=ea_music_get_pos();
            if(check_button(but_controls::dpad_right)) {
                Mix_SetMusicPosition(mypos*ea_music->len+1);
            }
            if(check_button(but_controls::dpad_left)) {
                Mix_SetMusicPosition(mypos*ea_music->len-1);
            }
            image_draw(albumart,0,32,32,
                       90.f/albumart->w,90.f/albumart->h,
                       (SDL_Color){255,255,255,255},0);
            draw_text(txt_title,128,32,0,0);
            draw_text(txt_artist,128,46,0,0);
            draw_text(txt_album,128,5,0,0);

            SDL_Rect myrect={64,245,352,16};
            SDL_Rect posrect={64,245,(int)round(352*(mypos)),16};
            if(mypos>=1) {if (selx<myfiles-1)selx++; else selx=0; ea_music_play_next(mydir+"/"+myfile[selx]);}

            SDL_SetRenderDrawColor(window->ren,100,100,105,255);
            SDL_RenderFillRect(window->ren,&myrect);
            SDL_SetRenderDrawColor(window->ren,200,200,210,255);
            SDL_RenderFillRect(window->ren,&posrect);


            break;
        }

        /// ONLINE LISTENING!!!!!!!!!!!!!!!!
        case 20:
        {
            draw_text(txt_onlisten,475,5,1,0);
            if(check_button_pressed(but_controls::o)) {level=0; playBackSound();}
            break;

        }
        default:break;
    }
}
