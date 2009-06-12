/*
  SDLTk

  Copyright (C) 2003-2005 John Beuving (john.beuving@beatforce.org)

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "SDL.h"
#include "SDL_Widget.h"


#if 0
SDL_Surface* SDL_WidgetGetBackground(SDL_Surface *dest,SDL_Rect *srcrect)
{
    Uint32 rmask, gmask, bmask, amask;
    SDL_Surface *newsurface;
    SDL_Surface *surface;
    SDL_Rect rect;

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;

    rect.x = srcrect->x;
    rect.y = srcrect->y;
    rect.w = srcrect->w;
    rect.h = srcrect->h;
    

    newsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, rect.w, rect.h, 32,
                                      rmask, gmask, bmask, amask);

    if(newsurface == NULL) 
    {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    surface = SDL_DisplayFormat(newsurface);
    SDL_FreeSurface(newsurface);

    if(SDL_BlitSurface(dest,&rect,surface,NULL)<0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

    return surface;

}
#endif

int SDL_WidgetIsInside(SDL_Widget *widget,int x, int y)
{
    if( x > widget->Rect.x)
        if(y > widget->Rect.y)
            if(x < (widget->Rect.x + widget->Rect.w))
                if(y < (widget->Rect.y + widget->Rect.h))
                {
                    if(widget->Clip.w > 0)
                    {
                        if(x > widget->Clip.x)
                            if(y > widget->Clip.y)
                                if(x < (widget->Clip.x + widget->Clip.w))
                                    if(y < (widget->Clip.y + widget->Clip.h))
                                        return 1;
                    }
                    else
                        return 1;
                }
    return 0;
}


/*
 * A function to calculate the intersection of two rectangles:
 * return true if the rectangles intersect, false otherwise
 */
int SDL_IntersectRect(const SDL_Rect *A, const SDL_Rect *B,SDL_Rect *intersection)
{
    Sint16 Amin, Amax, Bmin, Bmax;
    
    /* Horizontal intersection */
    Amin = A->x;
    Amax = Amin + A->w;
    Bmin = B->x;
    Bmax = Bmin + B->w;
    if(Bmin > Amin)
        Amin = Bmin;
    intersection->x = Amin;
    if(Bmax < Amax)
        Amax = Bmax;
    intersection->w = Amax - Amin > 0 ? Amax - Amin : 0;

    /* Vertical intersection */
    Amin = A->y;
    Amax = Amin + A->h;
    Bmin = B->y;
    Bmax = Bmin + B->h;
    if(Bmin > Amin)
        Amin = Bmin;
    intersection->y = Amin;
    if(Bmax < Amax)
        Amax = Bmax;
    intersection->h = Amax - Amin > 0 ? Amax - Amin : 0;
    
    return (intersection->w && intersection->h);
}

int SDL_RectInside(const SDL_Rect *A, const SDL_Rect *B)
{
    if(A->x >= B->x)
        if(A->y >= B->y)
            if(A->y <= B->y + B->h)
                if(A->x <= B->x + B->w)
                    if(A->x + A->w <= B->x + B->w)
                        if(A->y + A->h <= B->y + B->h)
                            return 1;
    return 0;
}
