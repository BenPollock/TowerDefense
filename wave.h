////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// wave.h
// Description: Wave is not a regular object and cannot be drawn.
// It serves as a convenient storage medium for variables to
// reduce the line count of track.cpp
////////////////////////////

#ifndef WAVE_H
#define WAVE_H

#include <iostream>

class Wave{

public:
    Wave(int x, int y, int z, int w, int a, float b, int c, int d);
    ~Wave();
    int getNum();               // returs num_c
    int getTime();              // returns time
    int getDelay();             // returns delay_time
    int getType();              // return type
    int getFlying();            // return flying
    float getSpeed();           // return speed
    int getHp();                // return hp
    int getTreasure();          // return treasure

private:
    int num_c;                  // number of creatures in this wave
    int time;                   // total time to release each creature
    int delay_time;             // time until next wave
    int type;                   // type of creature to be released
    int flying;                 // flying value of creatures (0 = ground)
    float speed;                // speed of the creatures
    int hp;                     // health of the creatures
    int treasure;               // money added upon creature death
};

#endif // WAVE_H
