/****************************************************************************
 Yihsiang Liow
 Copyright
 ****************************************************************************/
#ifndef IMAGE_H
#define IMAGE_H

#include "SDL.h"
#include "SDL_image.h"
//#include "SingletonSurface.h"
#include "Rect.h"

class Image
{
public:
    Image(const char * filename)
        : image(IMG_Load(filename)),
          rect_(Rect(image->clip_rect))
    {
      if (image == NULL)
      {
          printf("Error in Image::Image(): No image file %s\n", filename);
          exit(1);
      }
      sdl_rect = image->clip_rect;
      //surface = SingletonSurface::getInstance();
    }
    
    Image(SDL_Surface * s) // Used by text surfaces
      : image(s)
      {}
      
    ~Image()
	{
	  SDL_FreeSurface(image);
	}
      
      Rect getRect()
      {
	return Rect(image->clip_rect);
      }

    Rect rect() const
    {
        return rect_;
    }

    Rect & rect()
    {
        return rect_;
    }
      
      //private:
      //Surface * surface;
      SDL_Surface * image;
      SDL_Rect sdl_rect;
      Rect rect_;
};

#endif

