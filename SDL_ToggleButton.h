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

#ifndef __SDL_TOGGLEBUTTON_H__
#define __SDL_TOGGLEBUTTON_H__

#include <stdarg.h>

#include "SDLTk.h"

typedef struct SDL_ToggleButton
{
    SDL_Widget       Widget;
    SDL_Surface      *disabled;
    SDL_Surface      *normal;
    SDL_Surface      *highlighted;
    SDL_Surface      *pressed;
    SDL_eButtonState  State;

    SDL_Widget *Label;


}SDL_ToggleButton;

SDL_Widget* SDL_ToggleButtonNew();
void        SDL_ToggleButtonDraw(SDL_Widget *widget,SDL_Surface *dest,SDL_Rect *Area);
int         SDL_ToggleButtonProperties(SDL_Widget *widget,int feature,va_list list);
int         SDL_ToggleButtonEventHandler(SDL_Widget *widget,SDL_Event *event);
void        SDL_ToggleButtonClose(SDL_Widget *widget);


#endif /* __TOGGLEBUTTON_H__ */
