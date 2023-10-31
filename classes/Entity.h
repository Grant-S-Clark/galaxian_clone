//Name: Grant Clark
//Date: November 25th, 2021
//File: Entity.h

#ifndef ENTITY_H
#define ENTITY_H

#include "Includes.h"
#include "Surface.h"
#include "Constants.h"

const int SHIP = 0,
    ALIEN_B = 1,
    ALIEN_P = 2,
    ALIEN_R = 3,
    ALIEN_F = 4,
    SHIP_S = 5;

const int IMAGE_TOTAL = 6;

class Entity : public Rect
{
  public:
    Entity();
    Entity(const int);
    ~Entity();

    static void set_surface(Surface* &);
    void set_pos(const int, const int);
    void set_size(const int, const int);
    void set_color(const int, const int, const int);
    void set_image(const int);
    
    virtual void update();
    virtual void draw();

    Image   image() const;
    Image&  image();
    
    void set_rect(const int, const int,
                  const int, const int);
    Rect  rect() const;
    
    int  dx() const;
    int& dx();
    int  dy() const;
    int& dy();
    
  private:
    Image* image_;
    bool is_image_;
    int r_, g_, b_;
    int dx_, dy_;
    
    //Bounding rect used for collision checking.
    Rect rect_;

    //Surface pointer for drawing.
    static Surface * s_ptr_;

    static Image* images_[IMAGE_TOTAL];
};

#endif
