#pragma once
#include "main.h"

extern int prepare_system_sounds();

extern void playSelectSound();
extern void playPressSound();
extern void playBackSound();

extern void ea_music_get_metadata();

extern int ea_music_open(std::string filename);

extern void ea_music_play();

extern void ea_music_pause();
extern void ea_music_resume();

extern int ea_music_paused();

extern void ea_music_stop();

extern void ea_music_play_next(std::string myfile);

float ea_music_get_pos();
