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
#include "SDL_Widget.h"

typedef struct SDL_Margins
{
    short left;
    short top;
    short bottom;
    short right;
}SDL_Margins;

typedef struct SDL_ImageFrame
{
	SDL_Surface *surface;
	int delay;	
}SDL_ImageFrame;

typedef struct SDL_ImageTransition
{
	int time;
	int delay;
	int alpha;
	int how;
	void *Client;
	SDL_Surface			    *ResultSurface;
	SDL_Surface				*ToSurface; 
	SDL_Surface				*FromSurface; 
}SDL_ImageTransition;

typedef struct SDL_Image
{
    SDL_Surface *surface;
	int delay;


    SDL_Rect selection;
    SDL_Margins margin;
    char *filename;

	int framecount;
    int cur_frame;
	int loopcount;
	unsigned long old_time;
	SDL_ImageTransition		*Transition;
	
}
SDL_Image;

/* the transition */
/*
	each widget than can use transition should have a SDL_ImageTransition for each state.
	In this structure the how and how long is defined. Transition will be done to the
	TransSurface of the destination image. For each frame update the signal "frame" is send.
	Images must have the same size/margins.
	If transition time is 0 and update will be sent immediatly and other parameters will be ignored.
	SDL_ImageBlitToSurface will blit the TransSurface if available (including margins)
*/

SDL_Image *SDL_ImageLoad(char *filename, SDL_Rect *selection);
int SDL_ImageBlitToSurface(SDL_Image *image,const SDL_Rect *srcrect,SDL_Surface *surface,const SDL_Rect *destrect);
int SDL_ImageSetMargins(SDL_Image *image,short left, short top,short bottom,short right);
void SDL_ImageEvent(SDL_Event *Event);
int SDL_ImageGetHeight(SDL_Image *image);
int SDL_ImageGetWidth(SDL_Image *image);
void SDL_ImageStartTransition(SDL_Image *from,SDL_Image *to,SDL_ImageTransition *transition);
#endif /* __SDL_IMAGE_H__ */

