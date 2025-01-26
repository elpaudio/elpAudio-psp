#include "controls.h"

void reset_press() {
    for(int i=0;i<MAX_BUTTONS;i++) {buttons->butpress[i]=0;buttons->lastbutpress[i]=0;}
}
void check_press() {
	for (int b = 0; b < MAX_BUTTONS; b++) {
		buttons->lastbutpress[b] = buttons->butpress[b];
	}
    for (int i=0;i<MAX_BUTTONS;i++) { buttons->butpress[i]=SDL_GameControllerGetButton(window->joy,(SDL_GameControllerButton)i);}
}
int check_button_pressed(but_controls button) {
    return buttons->butpress[button] && !buttons->lastbutpress[button];
}
int check_button_released(but_controls button) {
    return !buttons->butpress[button] && buttons->lastbutpress[button];
}
int check_button(but_controls button) {
    return buttons->butpress[button] && buttons->lastbutpress[button];
}
