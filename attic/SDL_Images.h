/*
  SDLTk

  Image handling
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
#ifndef __SDL_IMAGE_H__
#define __SDL_IMAGE_H__

#include "SDL.h"


typedef struct SDL_Margins
{
    short left;
    short top;
    short bottom;
    short right;
}SDL_Margins;

typedef struct SDL_Image
{
    SDL_Surface *surface;
    SDL_Surface *surface1;
    SDL_Surface *surface2;
    SDL_Rect selection;
    SDL_Margins margin;
    char *filename;

    Uint8  alpha;
    Uint32 old_time;
    Uint32 total_time;
    int nof_frames;
    int cur_frame;
    int repeat_delay;
}
SDL_Image;


SDL_Image *SDL_ImageLoad(char *filename, SDL_Rect *selection);
int SDL_ImageBlitToSurface(SDL_Image *image,SDL_Surface *surface,const SDL_Rect *destrect);
int SDL_ImageSetMargins(SDL_Image *image,short left, short top,short bottom,short right);
void SDL_ImageEvent(SDL_Event *Event);

#endif /* __SDL_IMAGE_H__ */

