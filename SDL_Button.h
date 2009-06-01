/*
  SDLTk

  Button widget
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

#ifndef __SDL_BUTTON_H__
#define __SDL_BUTTON_H__

#include <stdarg.h>

#include "SDL.h"
#include "SDL_Images.h"
#include "SDL_Widget.h"


typedef enum SDL_eButtonState
{
    SDL_BUTTON_STATE_DOWN,
    SDL_BUTTON_STATE_UP,
    SDL_BUTTON_STATE_HIGHLIGHTED,
    SDL_BUTTON_STATE_FOCUSED
}SDL_eButtonState;

typedef struct SDL_Button
{
    SDL_Widget       Widget;
    SDL_eButtonState  State;

    SDL_Widget *Label;
    SDL_Font   *Font;

    int height;
}SDL_Button;

typedef struct SDL_ButtonStyleState
{
	SDL_Image			*Image;
	SDL_ImageTransition *Transition;
}SDL_ButtonStyleState;

typedef struct SDL_ButtonStyle
{
    SDL_ButtonStyleState        *Normal;
    SDL_ButtonStyleState        *Pressed;
    SDL_ButtonStyleState        *Focused;
    SDL_ButtonStyleState        *Highlighted;
    SDL_ButtonStyleState        *Disabled;
}SDL_ButtonStyle;


SDL_Widget* SDL_ButtonNew();
void        SDL_ButtonClose(SDL_Widget *widget);

/* Set a label to the button */
int SDL_ButtonSetLabel(SDL_Widget *widget,char *title);
int SDL_ButtonSetImage(SDL_Widget *widget,SDL_eButtonState state,SDL_Image *image);
int SDL_ButtonSetTransition(SDL_Widget *widget,SDL_eButtonState state,int how);

int SDL_ButtonSetFont(SDL_Widget *widget,SDL_Font *Font);
int SDL_ButtonSetFontHeight(SDL_Widget *widget,int height);
#endif /* __BUTTON_H__ */
