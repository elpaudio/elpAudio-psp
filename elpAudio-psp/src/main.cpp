#include "main.h"
#include "gamebreaker.hpp"
GBSprite *sprOnOff=nullptr;
GBFont *fntMainText=nullptr;

std::vector<GBText *> eaTexts;
std::vector<GBText *> eaMusicText;
ds_list off_files;
ds_list off_dirs;
str mydir="ms0:/MUSIC";
str mylastdir="";
int off_total_dirs=0;

GBMusic *eaMusic;

struct eaText {
    GBText *txt;
    int type;
};
std::vector<eaText> off_comp_text;

void dsmap(ds_map map, str key, void *value) {
    map.resize(map.size()+1);
    map[map.size()-1].key=key;
    map[map.size()-1].value=value;
}

void *dsmap_get(ds_map map,str key) {
    for(int i=0;i<map.size();i++) {
        if(map[i].key==key) return map[i].value;
    }
    return nullptr;
}

extern "C" int main(int argc, char **argv) {
    int l0_sel=0;
    int off_sel=0,
        off_scroll=0;
    int level=0;
    gb::init(0,0,480,272,"");

    eaMusic=new GBMusic;

    sprOnOff=sprite::add("data/images/offon.png",2,0,0);
    fntMainText=font::add("data/fonts/sourcesans.ttf",14);
    eaTexts.resize(10);
    eaMusicText.resize(4);
    draw::set_font(fntMainText);
    eaTexts[0]=new GBText("elpAudio");
    eaTexts[1]=new GBText("Online\n(coming soon)");
    eaTexts[2]=new GBText("Offline");
    eaTexts[3]=new GBText("Total directories: ");
    for(int i=0;i<3;i++) {
        eaMusicText[i]=new GBText("Untitled");
    }

    while(gb::running()) {
        draw::color_sdl(col::lt_gray);
        draw::rect(0,0,480,272,0);
        draw::color_sdl(col::gray);
        draw::rect(0,0,480,32,0);
        draw::rect(0,240,480,32,0);
        switch(level) {
            case 3: {
                draw::color_sdl(col::white);
                draw::text(64,4,eaMusicText[2]);
                draw::text(64,48,eaMusicText[1]);
                draw::text(64,72,eaMusicText[0]);

                break;
            }
            case 10: {
                if(mylastdir!=mydir) {
                    draw::color_sdl(col::white);
                    for(int i=0;i<off_comp_text.size();i++) {if(off_comp_text[i].txt!=nullptr)delete off_comp_text[i].txt;}
                    off_comp_text.clear();
                    mylastdir=mydir;
                    off_files=file::find::list(mydir,"*.mp3;*.ogg;*.wav;*.flac;*.it;*.mod;*.xm;*.s3m",0);
                    off_dirs=file::find::list(mydir,"*.*",fa::dir);
                    off_comp_text.resize(off_files.size()+off_dirs.size());
                    int i=0;
                    for(;i<off_dirs.size();i++) {
                        off_comp_text[i].txt=new GBText(off_dirs[i].data);
                        off_comp_text[i].type=off_dirs[i].type;
                    }
                    for(int ii=0;ii<off_files.size();ii++) {
                        off_comp_text[i].txt=new GBText(off_files[ii].data);
                        off_comp_text[i].type=off_files[ii].type;
                        i++;
                    }
                    off_total_dirs=off_dirs.size();
                    *eaTexts[3]="Total directories: "+stringify(off_total_dirs);
                }
                int off_file_size=off_comp_text.size();
                if(joystick::pressed(joystick::working(),joystick::button::circle)) level=0;
                if(joystick::pressed(joystick::working(),joystick::button::dpad_down)) {
                    if(off_sel<off_file_size-1) off_sel++;
                    else off_sel=0;
                }
                if(joystick::pressed(joystick::working(),joystick::button::dpad_up)) {
                    if(off_sel>0) off_sel--;
                    else off_sel=off_file_size-1;
                }

                if(joystick::pressed(joystick::working(),joystick::button::cross)) {
                    switch(off_comp_text[off_sel].type) {
                        case file::type::tfile: {
                            level=3;
                            eaMusic=music::add(mydir+"/"+off_comp_text[off_sel].txt->txt,0);
                            music::play(eaMusic);
                            for(int i=0;i<3;i++) *eaMusicText[i]=eaMusic->tag[i];
                            break;

                        }
                        case file::type::tdir:
                            mydir+="/"+off_comp_text[off_sel].txt->txt;
                            off_sel=0;
                            break;
                    }

                }
                if(joystick::pressed(joystick::working(),joystick::button::triangle)) mydir=file::path_parent(mydir);
                off_scroll=(off_sel<6)?0:(off_sel>off_file_size)?off_file_size-5:off_sel-5;
                draw::set_text_align(1,0);
                draw::text(480,32,eaTexts[3]);
                draw::set_text_align(0,0);
                for(int i=0;i<math::clamp(off_file_size-off_scroll,0,10);i++) {
                    draw::color_sdl(col::white);
                    if(off_sel==i+off_scroll) {draw::color_sdl(col::dk_gray);}
                    draw::text(128,64+i*16,off_comp_text[i+off_scroll].txt);
                }
                draw::color_sdl(col::white);
            break;
            }
            case 0:{
                if(
                    joystick::pressed(joystick::working(),joystick::button::dpad_right)||
                    joystick::pressed(joystick::working(),joystick::button::dpad_left)) {l0_sel=!l0_sel;}

                if(joystick::pressed(joystick::working(),joystick::button::cross)) {
                    if(l0_sel==0) level=20;
                    else {
                        level=10;
                    }
                }

                Uint8 mycol=200+math::lendir_x(55,current_time*0.5);
                SDL_Color myrealcol={mycol,mycol,mycol,255};
                for(int i=0;i<2;i++) {
                    draw::color_sdl(col::white);
                    draw::color_sdl(col::gray);
                    draw::rect((480-256)/3+i*(480-256),48,128,160,0);
                    if(l0_sel==i) {draw::color_sdl(myrealcol);draw::rect((480-256)/3+i*(480-256),48,128,160,1);}
                    else {
                        draw::color_sdl(col::white);
                    }
                    draw::sprite(sprOnOff,i,(480-256)/3+i*(480-256),48,1,1,0);
                }
                draw::color_sdl(col::white);
                draw::text(5,2,eaTexts[0]);
                draw::text((float)(480-256)/3+32,144,eaTexts[1]);
                draw::text((float)(480-256)/3+(480-256)+32,144,eaTexts[2]);
                break;}
        }

        screen::draw(60);
    }
    gb::shutdown();
    return 0;

}
