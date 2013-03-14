#include "wave.h"

// Constructor
Wave::Wave(int x, int y, int z, int w, int a, float b, int c, int d){
    num_c = x;
    time = y;
    delay_time = z;
    type = w;
    flying = a;
    speed = b;
    hp = c;
    treasure = d;
}
//Destructor
Wave::~Wave(){
}

// Return number of creatures
int Wave::getNum(){
    return num_c;
}

// Return release time
int Wave::getTime(){
    return time;
}

// Return time until next wave
int Wave::getDelay(){
    return delay_time;
}

// Return the type of creatures
int Wave::getType(){
    return type;
}

// Return the flying value of creatures
int Wave::getFlying(){
    return flying;
}

// Return speed of creatures
float Wave::getSpeed(){
    return speed;
}

// Return health of creatures
int Wave::getHp(){
    return hp;
}

// Return treasure value of creatures
int Wave::getTreasure(){
    return treasure;
}
