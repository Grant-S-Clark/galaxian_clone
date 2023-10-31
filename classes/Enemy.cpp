//Name: Grant Clark
//Date: November 25th, 2021
//File: Enemy.cpp

#include "Enemy.h"

//Starts out as zero.
int Enemy::offset_ = 0;
bool Enemy::moving_left_ = false;


Enemy::Enemy(const E_Type t) : t_(t), d_s_(None),
                               diving_(false),
                               in_group_(false),
                               sin_val(0.0), rand_sin_mult(0.0),
                               rand_sin_inc(0.0),
                               flagship_minions(nullptr),
                               adjust_minions(false),
                               firing_(false),
                               firing_interval(0),
                               firing_counter(0),
                               laser_total(0)
{
    switch (t_)
    {
        case Blue:
            set_image(ALIEN_B);
            break;
        case Purple:
            set_image(ALIEN_P);
            break;
        case Red:
            set_image(ALIEN_R);
            break;
        case Flagship:
            set_image(ALIEN_F);
            flagship_minions = new Enemy*[3];
            
            break;
    }
    
    return;
}
             

Enemy::~Enemy()
{
    if (flagship_minions != nullptr)
        delete[] flagship_minions;
    
    return;
}


void Enemy::update()
{
    //If the enemy is diving.
    if (diving_)
    {
        switch (d_s_)
        {
            case None:
            {
                //Set up initial dive values.
                d_s_ = Begin;
                //Save initial position.
                start_x = x - offset_;
                start_y = y;
                sin_val = 0;
                dx() = 2;
                if (rand() % 2)
                    dx() *= -1;

                if (t_ == Flagship)
                    adjust_minions = true;
                
                break;
            }
            case Begin:
            {
                //Performing the parabola beginning of the dive.
                sin_val += .1;
            
                dy() = -1 * cos(sin_val) * 4;
                
                if (y > start_y)
                {
                    d_s_ = Dive;
                    sin_dive_ = rand() % 2;
                    if (sin_dive_)
                    {
                        sin_val = 0;
                        rand_sin_mult = (rand() % 75 + 40) / 10.0;
                        rand_sin_inc = (rand() % 10 + 7) / 100.0;
                        dy() = 3;
                        dx() = 0;
                    }
                    else
                    {
                        dy() = rand() % 5 + 2;
                        dx() = rand() % 5 + 2;
                        if (rand() % 2 == 0)
                            dx() *= -1;
                    }

                    if (t_ == Flagship)
                        adjust_minions = true;

                    firing_interval = 40 - (rand() % (10 * (int(t_) + 1)));
                    laser_total = rand() % 4 + 1;
                    firing_counter = 0;
                    
                }
                
                break;
            }

            case Dive:
            {
                if (sin_dive_)
                {
                    sin_val += rand_sin_inc;
                    dx() = -1 * cos(sin_val) * rand_sin_mult;
                }

                if (laser_total > 0 &&
                    firing_counter == firing_interval)
                {
                    firing_counter = 0;
                    --laser_total;
                    firing_ = true;
                }
                else
                {
                    ++firing_counter;
                    firing_ = false;
                }

                if (y > SCREEN_H)
                {
                    dy() = 3;
                    dx() = 0;
                    y = h * -2;
                    x = start_x + offset_;
                    d_s_ = Return;
                    firing_ = false;
                }
                
                break;
            }

            case Return:
            {
                if (moving_left_)
                    dx() = -1;
                else
                    dx() = 1;
            
                if (y >= start_y)
                {
                    y = start_y;
                    dy() = 0;
                    d_s_ = None;
                    diving_ = false;
                    in_group_ = false;
                }

                break;
            }   
        }

        if (adjust_minions)
        {
            for (int i = 0; i < 3; i++)
                if (flagship_minions[i] != nullptr &&
                    flagship_minions[i]->in_group())
                {
                    flagship_minions[i]->dx() = dx();
                    flagship_minions[i]->dy() = dy();
                    flagship_minions[i]->update_sin_vals(sin_val,
                                                         rand_sin_mult,
                                                         rand_sin_inc,
                                                         sin_dive_);
                }
            adjust_minions = false;
        }
       
        //Screenwrapping.
        if (dx() > 0 && x >= SCREEN_W)
            x = w * -1;
        else if (dx() < 0 && x < w * -1)
            x = SCREEN_W;
    }

    else
    {
        //Set velocity.
        if (moving_left_)
            dx() = -1;
        else
            dx() = 1;
    }
    
    return;
}


void Enemy::move()
{
    //Move the entity.
    Entity::update();

    //Update bouding box.
    switch (t_)
    {
        //w = 22, h = 16
        case Blue:
        case Purple:
        case Red:
            set_rect(this->x + 5, this->w - 5,
                     this->y + 8, this->h - 8);
            break;
            //w = 22, h = 22
        case Flagship:
            set_rect(this->x + 5, this->w - 5,
                     this->y + 5, this->h - 5);
            break;
    }

    return;
}


int Enemy::offset()
{
    return offset_;
}


void Enemy::update_offset()
{
    if (moving_left_)
        offset_--;
    else
        offset_++;
}


void Enemy::change_direction()
{
    moving_left_ = !moving_left_;

    return;
}


void Enemy::reset_statics()
{
    offset_ = 0;
    moving_left_ = false;

    return;
}
