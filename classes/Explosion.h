//Name: Grant Clark
//Date: November 12th, 2021
//File: Explosion.h

#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Surface.h"

class Explosion
{
  public:
    Explosion(const int, const int, const int);
    ~Explosion();

    static void set_surface(Surface* &);
    
    bool finished();
    void draw();
  private:
    int x_, y_;
    int rad_, mid_rad_;
    int r_, g_, b_;
    
    //Determines if the explosion has
    //completed its animation.
    bool finished_;
    bool color_down_;

    static Surface* s_ptr_;
};

#endif
