/*
  SDLTk

  one line to give the program's name and an idea of what it does.
  Copyright (C) 2005-2006 John Beuving (john.beuving@beatforce.org)

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

#ifndef __SDL_KNOB_H__
#define __SDL_KNOB_H__

#include "SDL_Widget.h"

typedef struct SDL_Knob
{
    SDL_Widget Widget;
    short   Value;
    int     Min;
    int     Max;
    Uint32  color;

    int     DotRadius;
    int     Radius;

    int     StartAngle;
    int     AngleRange;
}SDL_Knob;

enum
{
    KNOB_CURRENT_VALUE,
    KNOB_MAX_VALUE,
    KNOB_MIN_VALUE,
};

SDL_Widget*  SDL_KnobNew();

int SDL_KnobSetValue(SDL_Widget *widget,int type,int value);


#endif /*  __SDL_KNOB_H__ */
