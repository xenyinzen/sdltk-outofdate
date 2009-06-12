/*
  SDLTk

  Slider widget definition
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

#ifndef __SDL_SLIDER_H__
#define __SDL_SLIDER_H__

#include "SDL_Widget.h"
#include "SDL_WidTool.h"

#define SLIDER_IDLE      0x00
#define SLIDER_DRAG      0x01

#define HORIZONTAL 1
#define VERTICAL   2

typedef struct SDL_Slider
{
    SDL_Widget  Widget; /* Parent object */


    SDL_Image   *SliderBar;
    SDL_Image   *SliderButton;
    SDL_Image   *SliderButtonPressed;

    int SliderButtonWidth;
    int SliderButtonHeight;

    int orientation;  /* hor or vert */
    int percentage;   /* percentage on scale */
    int pixeloffset;  /* pixeloffset runs from 0 to (Slider height - SliderButton height) left nd top */
    
    int state;      /* states of the widget, used for eventhandler */
    int changed;    /* if changed is true a redraw is performed */


                    /* Slider range (Can be queried by event callback functions */
    int    MaxValue;        /* maximum value (percentage = 100%) */
    int    MinValue;        /* minimum valie (percentage = 0%) */
    int    CurrentValue;    /* value of current position */

    int ValueLocked;
    
    int NormalStepSize;

}SDL_Slider;

enum
{
    SLIDER_CURRENT_VALUE,
    SLIDER_MAX_VALUE,
    SLIDER_MIN_VALUE,
    SLIDER_NORMAL_STEP_SIZE
};

enum
{
    SLIDER_IMAGE_NORMAL_THUMB,
    SLIDER_IMAGE_NORMAL_BAR,
    SLIDER_IMAGE_PRESSED_THUMB
};

/* prototypes */
SDL_Widget* SDL_SliderNew();


int SDL_SliderGetState(SDL_Widget *widget);


int SDL_SliderSetImage(SDL_Widget *widget,int which,SDL_Image *image);
int SDL_SliderSetValue(SDL_Widget *widget,int type,int value);

/* internal enums */
enum
{
    RIGHT,
    LEFT,
    UP,
    DOWN
};

enum
{
    SMALL_STEP,
    NORMAL_STEP,
    LARGE_STEP
};

int SDL_SliderGetCurrentValue(SDL_Widget *widget);
int SDL_SliderSetMaxValue(SDL_Widget *widget,int MaxValue);

#endif /* __SDL_SLIDER_H__ */
