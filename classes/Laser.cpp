//Name: Grant Clark
//Date: November 23rd, 2021
//File: Laser.cpp

#include "Laser.h"

Laser::Laser(bool player)
{
    //If it was fired by a player.
    if (player)
        set_color(255, 255, 0);
    else
        set_color(255, 0, 0);
    
    set_size(2, 10);
    if (player)
        dy() = -3;
    else
        dy() = 2;
    
    return;
}

Laser::~Laser()
{
    return;
}
