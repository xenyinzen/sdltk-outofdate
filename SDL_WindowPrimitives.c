/*
  SDLTk

  Implementation of window specific drawing
  Copyright (C) 2003-2006 John Beuving (john.beuving@beatforce.org)

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
#include "SDLTk.h"
#include "SDL_GLPrimitives.h"
#include "SDL_WindowPrimitives.h"



int SDL_DrawLineWindow(SDL_Window *window, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
    x1 +=  window->Widget.Rect.x;
    x2 +=  window->Widget.Rect.x;
    
    y1 +=  window->Widget.Rect.y;
    y2 +=  window->Widget.Rect.y;

    if(window->surface->flags & SDL_OPENGL)
        return 0;
    else
        return SDL_DrawLine(window->surface,x1,y1,x2,y2,color);
}

int SDL_DrawCircleWindow(SDL_Window *window, Sint16 x, Sint16 y, Sint16 r, Uint32 color)
{
    x +=  window->Widget.Rect.x;
    y +=  window->Widget.Rect.y + 10;

    if(window->surface->flags & SDL_OPENGL)
        return 0;
    else
        return SDL_DrawCircle(window->surface,x,y,r,color);
}


void SDL_FillRectWindow(SDL_Window *window,SDL_Rect *r,Uint32 color)
{
    SDL_Rect c;

    c.x = r->x + window->Widget.Rect.x;
    c.y = r->y + window->Widget.Rect.y;
    c.w = r->w;
    c.h = r->h;
	
    if(window->surface->flags & SDL_OPENGL)
        printf("");
        /* SDL_GLFillRect(window->surface,&c,color); */
    else
        SDL_FillRect(window->surface,&c,color);            
}


void SDL_BlitToWindow(SDL_Surface *src,SDL_Rect *srcrect, SDL_Window *window,SDL_Rect *destrect)
{
    SDL_Surface *dest = window->surface;
    SDL_Rect wr;

    wr.x = destrect->x + window->Widget.Rect.x;
    wr.y = destrect->y + window->Widget.Rect.y;
    wr.w = destrect->w;
    wr.h = destrect->h;

    SDL_BlitSurface(src,srcrect,dest,&wr);
}



int SDL_DrawBoxWindow(SDL_Window *window, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
    color = color << 8;
    color |= 0xff;

    x1+=window->Widget.Rect.x;
    y1+=window->Widget.Rect.y;
    x2+=window->Widget.Rect.x;
    y2+=window->Widget.Rect.y;
	
    return boxColor(window->surface,x1,y1,x2,y2,color);
}

int SDL_DrawRectangleWindow(SDL_Window *window, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
    color = color << 8;
    color |= 0xff;

    x1+=window->Widget.Rect.x;
    y1+=window->Widget.Rect.y;
    x2+=window->Widget.Rect.x;
    y2+=window->Widget.Rect.y;
	
    return rectangleColor(window->surface,x1,y1,x2,y2,color);
}
