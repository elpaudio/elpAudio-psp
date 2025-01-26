#include "sounds.h"
#include <SDL2/SDL_mixer.h>
//#include <FLAC++/decoder.h>

Mix_Chunk *sndSelect;
Mix_Chunk *sndPress;
Mix_Chunk *sndBack;

music *ea_music;

int prepare_system_sounds() {
    sndSelect=Mix_LoadWAV("data/sound/select.wav");
    sndPress=Mix_LoadWAV("data/sound/press.wav");
    sndBack=Mix_LoadWAV("data/sound/back.wav");
    ea_music=new music;
    ea_music->album="";
    ea_music->artist="";
    ea_music->date="";
    ea_music->name="";
    ea_music->number="";
    ea_music->len=-1;
    ea_music->pos=0;
    ea_music->loops=0;
    ea_music->mus=NULL;
    return 1;
}

void playSelectSound() {
    Mix_PlayChannel(-1,sndSelect,0);
}

void playPressSound() {
    Mix_PlayChannel(-1,sndPress,0);
}
void playBackSound() {
    Mix_PlayChannel(-1,sndBack,0);
}

void ea_music_get_metadata() {
    ea_music->album=strcasecmp(Mix_GetMusicAlbumTag(ea_music->mus),"")==0 ? "Unknown album" : Mix_GetMusicAlbumTag(ea_music->mus);
    ea_music->artist=strcasecmp(Mix_GetMusicArtistTag(ea_music->mus),"")==0 ? "Unknown artist" : Mix_GetMusicArtistTag(ea_music->mus);
    ea_music->name=strcasecmp(Mix_GetMusicTitle(ea_music->mus),"")==0 ? ea_music->fname : Mix_GetMusicTitle(ea_music->mus);
    *txt_title=ea_music->name;
    *txt_artist=ea_music->artist;
    *txt_album=ea_music->album;
}

int ea_music_open(std::string filename) {
    if(ea_music->mus!=NULL) {Mix_HaltMusic(); ea_music->mus=NULL;}
    ea_music->mus=Mix_LoadMUS(filename.c_str());
    ea_music->fname=filename;
    ea_music_get_metadata();
    return (ea_music->mus!=NULL);
}

void ea_music_play() {
    Mix_PlayMusic(ea_music->mus,ea_music->loops);
    ea_music->len=Mix_MusicDuration(ea_music->mus);
}

void ea_music_stop() {
    Mix_PauseMusic();
    Mix_SetMusicPosition(0);
}
void ea_music_pause() {
    Mix_PauseMusic();
}
void ea_music_resume() {
    Mix_ResumeMusic();
}

int ea_music_paused() {
    return Mix_PausedMusic();
}

void ea_music_play_next(std::string myfile) {
    ea_music_stop();
    ea_music_open(myfile);
    ea_music_play();
}

float ea_music_get_pos() {
    ea_music->pos=Mix_GetMusicPosition(ea_music->mus);
    return ea_music->pos/ea_music->len;
}
