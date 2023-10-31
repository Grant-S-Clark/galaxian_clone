//Name: Grant Clark
//Date: November 12th, 2021
//File: Background.cpp

#include "Background.h"

Surface* Background::s_ptr_ = nullptr;

Background::Background()
{
    //Create 60 total stars.
    for (int i = 0; i < 60; i++)
    { 
        stars.push_back(Star(rand() % (SCREEN_W - 2),
                             rand() % (SCREEN_H - 1)));
    }

    moving = true;
    
    return;
}


Background::~Background()
{
    return;
}


void Background::set_surface(Surface* & s_ptr)
{
    s_ptr_ = s_ptr;

    return;
}

void Background::update()
{
    //Move downward.
    for (int i = 0; i < stars.size(); i++)
    {
        if (moving)
            stars[i].y++;
        if (stars[i].y == SCREEN_H - 1)
        {
            stars.erase(stars.begin() + i);
            stars.push_back(Star(rand() % (SCREEN_W - 2), 0));
            //Move backwards in the checks so the star after the one
            //that was erased is not skipped.
            i--;
        }
        else if (stars[i].timer != stars[i].reset_time)
        {
            stars[i].timer++;
        }
        else
        {
            stars[i].timer = 0;
            stars[i].hidden = !stars[i].hidden;
        }
    }
    
    return;
}

void Background::draw()
{
    for (int i = 0, x, y, r, g, b; i < stars.size(); i++)
    {
        if (!stars[i].hidden)
        {
            x = stars[i].x;
            y = stars[i].y;
            r = stars[i].r;
            g = stars[i].g;
            b = stars[i].b;

            for (int j = 0; j <= 1; j++)
                for (int k = 0; k <= 1; k++)
                    s_ptr_->put_pixel(x + j, y + k, r, g, b);
        }
    }
    
    return;
}
