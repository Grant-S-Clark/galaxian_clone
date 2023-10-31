//Name: Grant Clark
//Date: November 12th, 2021
//File: Explosion.cpp

#include "Explosion.h"

Surface* Explosion::s_ptr_ = nullptr;

Explosion::Explosion(const int x,
                     const int y,
                     const int rad) :
    x_(x), y_(y), rad_(rad),
    r_(195), g_(120), b_(0),
    finished_(false),
    color_down_(false),
    mid_rad_(0)
{
    return;
}


Explosion::~Explosion()
{
    return;
}


void Explosion::set_surface(Surface* & s_ptr)
{
    s_ptr_ = s_ptr;

    return;
}


bool Explosion::finished()
{
    return finished_;
}


void Explosion::draw()
{
    s_ptr_->put_circle(x_, y_, rad_,
                       r_, g_, b_);

    if (!color_down_)
    {
        r_ += 15;
            
        if (r_ >= 255)
        {
            color_down_ = true;
        }
        
    }
    
    else
    {
        if (r_ >= 5)
            r_ -= 5;
    }

    if (g_ >= 5)
        g_ -= 5;

    s_ptr_->put_circle(x_, y_, mid_rad_ / 2,
                       0, 0, 0);
    mid_rad_++;

    if (mid_rad_ == (rad_ * 2) + 1)
        finished_ = true;
        
    return;
}
