#pragma once
#include "main.h"

enum but_controls {
    invalid = -1,
    x,
    o,
    p,
    d,
    sel,
    unused1,
    start,
    unused2,
    unused3,
    l,
    r,
    dpad_up,
    dpad_down,
    dpad_left,
    dpad_right,
    unused4
};


extern void reset_press();
extern void check_press();
extern int check_button_pressed(but_controls button);
extern int check_button_released(but_controls button);
extern int check_button(but_controls button);
