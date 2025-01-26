#include "maths.h"

double lengthdir_x(double len, int dir) {
    return cos(dir%360)*len;
}

double lengthdir_y(double len, int dir) {
    return sin(dir%360)*len;
}
double clamp(double val, double min, double max) {return (val<min) ? min : (val>max) ? max : val;}
