/*
  Beatforce/SDLTk

  one line to give the program's name and an idea of what it does.
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

#ifndef __SDL_FRAME_H__
#define __SDL_FRAME_H__

#include "SDL_Widget.h"
#include "SDL_Font.h"

typedef struct SDL_Frame
{
    SDL_Widget Widget;
    SDL_Image *TitleLeft;
	SDL_Image *TitleCenter;
	SDL_Image *TitleRight;
	SDL_Image *Left;
	SDL_Image *Right;
	SDL_Image *BottomLeft;
	SDL_Image *BottomRight;
	SDL_Image *Bottom;
    Uint32  color;
}SDL_Frame;


SDL_Widget*  SDL_FrameNew();


int SDL_FrameSetImage(SDL_Widget *widget,int which,SDL_Image *image);


enum
{
    FRAME_TITLE_CENTER,
    FRAME_TITLE_LEFT,
    FRAME_TITLE_RIGHT,
    FRAME_LEFT,
    FRAME_RIGHT,
    FRAME_BOTTOM_LEFT,
    FRAME_BOTTOM_RIGHT,
    FRAME_BOTTOM
};
#endif /*  __SDL_PANEL_H__ */
