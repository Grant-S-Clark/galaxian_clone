//Name: Grant Clark
//Date: November 25th, 2021
//File: Enemy.h

#ifndef ENEMY_H
#define ENEMY_H

#include <cstdlib>
#include <cmath> //For sin() function.
#include "Entity.h"

//Determines the type of enemy.
enum E_Type {
    Blue = 0, //0
    Purple,   //1
    Red,      //2
    Flagship  //3
};

/*
  Determines what stage of the dive the enemy is in.
  
  Begin:  The enemy is doing a slight upward parabola at the
          beginning of the dive.
  Dive:   The enemy is diving down towards the player in a
          random sin wave type fashion.
  Return: The enemy is returning to its position in the
          alien formation.
*/
enum Dive_Stage {
    None = 0,
    Begin,
    Dive,
    Return
};

class Enemy : public Entity
{
  public:
    Enemy(const E_Type);
    ~Enemy();

    E_Type t() const { return t_; };
    bool diving() const { return diving_; }
    bool & diving() { return diving_; }
    bool in_group() const { return in_group_; }
    bool & in_group() {return in_group_; }
    bool firing() const { return firing_; }
    void update() override;
    void move();

    Enemy** minions() { return flagship_minions; }
    
    static int  offset();
    static void update_offset();
    static void change_direction();
    static void reset_statics();
    
  private:
    void update_sin_vals(double s_v, double r_s_m,
                         double r_s_i, bool s_d)
    {
        sin_val = s_v;
        rand_sin_mult = r_s_m;
        rand_sin_inc = r_s_i;
        sin_dive_ = s_d;

        return;
    }
    
    E_Type t_;
    Dive_Stage d_s_;
    
    //Determines whether or not the enemy is diving down
    //at the player.
    bool diving_;

    //Determines if this enemy should be allowed to be edited
    //by a flagship formation.
    bool in_group_;
    
    //If true, dives in a sin wave, if false dives in a line.
    bool sin_dive_;
    
    //Position of alien before beginning a dive.
    int start_x, start_y;
    
    //sin_val is the value sent to the sin function to determine dx_,
    //and rand_sin_increment will increment sin_val.
    double sin_val;
    
    double rand_sin_mult, rand_sin_inc;

    //Flagship minion related objects.
    Enemy** flagship_minions;
    bool adjust_minions;

    //Laser firing related objects.
    bool firing_;
    int firing_interval;
    int firing_counter;
    int laser_total;
    
    //Offset used to move alien formation.
    static int offset_;
    
    //If true, moving left. If false, moving right.
    static bool moving_left_;
};

#endif
