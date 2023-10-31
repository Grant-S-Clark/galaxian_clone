//Name: Grant Clark
//Date: November 23rd, 2021
//File: Ship.cpp

#include "Ship.h"

Ship::Ship(const int type) :
    Entity(type)
{
    set_pos(SCREEN_W / 2 - (w / 2),
            SCREEN_H - (SCREEN_H / 10) -
            (h / 2));

    return;
}


Ship::~Ship()
{
    return;
}


void Ship::update()
{
    set_rect(this->x + 5, this->w - 5,
                this->y, this->h);
    Entity::update();

    return;
}
