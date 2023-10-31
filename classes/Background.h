//Name: Grant Clark
//Date: November 12th, 2021
//File: Background.h

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>
#include "Surface.h"
#include "Constants.h"

struct Star
{
    int r, g, b;
    int x, y;
    bool hidden;
    int timer;
    int reset_time = rand() % 100 + 60;

    Star(int x_, int y_)
    {
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;

        //Random true / false.
        hidden = static_cast<bool>(rand() % 2);
        timer = rand() % reset_time;

        //Location.
        x = x_;
        y = y_;
         
        return;
    }
};

class Background {
public:
    Background();
    ~Background();

    static void set_surface(Surface* &);
    
    void update();
    void draw();
private:
    std::vector<Star> stars;
    bool moving;

    static Surface* s_ptr_;
};


#endif
