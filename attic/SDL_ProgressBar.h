/*
  SDLTk

  one line to give the program's name and an idea of what it does.
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

#ifndef __SDL_PROGRESSBAR_H__
#define __SDL_PROGRESSBAR_H__

#include "SDL_Widget.h"
#include "SDL_Font.h"

typedef struct SDL_ProgressBar
{
    SDL_Widget  Widget;
    SDL_Font   *Font;
    
    Uint32  color1;
    Uint32  color2;
    Uint32  color3;
    Uint32  color4;
    
    int State;

    int Orientation;

    int MinValue;
    int MaxValue;
    int CurrentValue;

    /* Settings for internal draw functions */
    int CurrentLine;   /* line where the volume is at */

}SDL_ProgressBar;

#define HORIZONTAL 1
#define VERTICAL   2

#define PROGRESSBAR_NORMAL 3
#define PROGRESSBAR_DRAG   4


SDL_Widget* SDL_ProgressBarNew();

int SDL_ProgressBarSetMaxValue(SDL_Widget *widget, int max);
int SDL_ProgressBarSetCurrentValue(SDL_Widget *widget,int value);
double SDL_ProgressBarGetCurrentValue(SDL_Widget *widget);
int SDL_ProgressBarGetState(SDL_Widget *widget);

#endif /* __SDL_PROGRESSBAR_H__ */
