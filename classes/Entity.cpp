//Name: Grant Clark
//Date: November 25th, 2021
//File: Entity.cpp

#include "Entity.h"

Surface* Entity::s_ptr_ = nullptr;

Image* Entity::images_[IMAGE_TOTAL] = {
    new Image("images/galaxian/GalaxianGalaxip.gif"),
    new Image("images/galaxian/GalaxianAquaAlien.gif"),
    new Image("images/galaxian/GalaxianPurpleAlien.gif"),
    new Image("images/galaxian/GalaxianRedAlien.gif"),
    new Image("images/galaxian/GalaxianFlagship.gif"),
    new Image("images/galaxian/GalaxianGalaxipSmall.gif")
};

Entity::Entity() : image_(nullptr), is_image_(false),
                   dx_(0), dy_(0)
{
    return;
}


//Set an image by passing a filename.
Entity::Entity(const int i) :
    image_(images_[i]), is_image_(true),
    dx_(0), dy_(0)
{
    x = image_->getRect().x;
    y = image_->getRect().y;
    w = image_->getRect().w;
    h = image_->getRect().h;
    
    return;
}


Entity::~Entity()
{
    return;
}


void Entity::set_surface(Surface* & s_ptr)
{
    s_ptr_ = s_ptr;

    return;
}


void Entity::set_pos(const int new_x, const int new_y)
{
    x = new_x;
    y = new_y;
    
    return;
}


void Entity::set_size(const int new_w, const int new_h)
{
    w = new_w;
    h = new_h;
    
    return;
}


void Entity::set_color(const int r, const int g, const int b)
{
    r_ = r;
    g_ = g;
    b_ = b;
    
    return;
}


void Entity::set_image(const int i)
{
    image_ = images_[i];

    x = image_->getRect().x;
    y = image_->getRect().y;
    w = image_->getRect().w;
    h = image_->getRect().h;
    
    is_image_ = true;
}


void Entity::draw()
{
    if (is_image_)
        s_ptr_->put_image(*image_, *this);
    else
        s_ptr_->put_rect(x, y, w, h, r_, g_, b_);
    
    return;
}


void Entity::update()
{
    x += dx_;
    y += dy_;

    return;
}


Image Entity::image() const
{
    return *image_;
}


Image& Entity::image()
{
    return *image_;
}


void Entity::set_rect(const int new_x, const int new_w,
                      const int new_y, const int new_h)
{
    rect_.x = new_x;
    rect_.w = new_w;
    rect_.y = new_y;
    rect_.h = new_h;

    return;
}


Rect Entity::rect() const
{
    return rect_;
}


int Entity::dx() const
{
    return dx_;
}

int& Entity::dx()
{
    return dx_;
}


int Entity::dy() const
{
    return dy_;
}

int& Entity::dy()
{
    return dy_;
}


