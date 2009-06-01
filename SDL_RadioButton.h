/*
  SDLTk

  Declaration of widget radiobutton
  Copyright (C) 2006 John Beuving (john.beuving@beatforce.org)

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

#ifndef __SDL_RADIOBUTTON_H__
#define __SDL_RADIOBUTTON_H__

#include "SDL_Widget.h"
#include "SDL_Font.h"
#include "SDL_Images.h"

typedef struct SDL_RadioButton
{
    SDL_Widget Widget;
    Uint32  color;
    int State;
    SDL_Widget *Label;
}SDL_RadioButton;

typedef struct SDL_RadioButtonStyle
{
    SDL_Image        *Normal;
    SDL_Image        *Selected;
    SDL_Image        *Focused;
    SDL_Image        *Highlighted;
    SDL_Image        *Disabled;
}SDL_RadioButtonStyle;

SDL_Widget*  SDL_RadioButtonNew();
void SDL_RadioButtonClose( SDL_Widget *widget );

int SDL_RadioButtonSetText(SDL_Widget *widget,char *text);
int SDL_RadioButtonSetImage(SDL_Widget *widget,int which,SDL_Image *image);

#endif /*  __SDL_RADIOBUTTON_H__ */
