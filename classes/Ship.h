//Name: Grant Clark
//Date: November 23rd, 2021
//File: Ship.h

#ifndef SHIP_H
#define SHIP_H

#include "Entity.h"

class Ship : public Entity
{
  public:
    Ship(const int type = SHIP);
    ~Ship();

    void update() override;
  private:
};

#endif
