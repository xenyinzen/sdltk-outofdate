/*
  SDLTk

  Scrollbar widget
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

#ifndef __SDL_SCROLLBAR_H__
#define __SDL_SCROLLBAR_H__

#include "SDL_Widget.h"
#include "SDL_WidTool.h"

#define SCROLLBAR_IDLE                    0x00
#define SCROLLBAR_DRAG                    0x01
#define SCROLLBAR_ARROW1_PRESSED          0x02
#define SCROLLBAR_ARROW1_HIGHLIGHTED      0x03
#define SCROLLBAR_ARROW2_PRESSED          0x04
#define SCROLLBAR_ARROW2_HIGHLIGHTED      0x05
#define SCROLLBAR_THUMB_PRESSED           0x06
#define SCROLLBAR_THUMB_HIGHLIGHTED       0x07
#define SCROLLBAR_SCROLLBAR_PRESSED       0x08
#define SCROLLBAR_SCROLLBAR_HIGHLIGHTED   0x09


#define HORIZONTAL 1
#define VERTICAL   2

typedef struct SDL_Scrollbar
{
    SDL_Widget  Widget; /* Parent object */
   
    int Orientation;  /* hor or vert */
   
    int State;      /* states of the widget */

    
    int    MaxValue;        /* maximum value (percentage = 100%) */
    int    MinValue;        /* minimum valie (percentage = 0%) */
    int    CurrentValue;    /* value of current position */
    int    NormalStepValue;    /* value of a normal step */
    int    SmallStepValue;    /* value of a normal step */
    
    int    PixelValue;

    int    offset;

}SDL_Scrollbar;

typedef struct SDL_ScollbarStyleState
{
    SDL_Image *Arrow1;
    SDL_Image *Arrow2;
    SDL_Image *Scrollbar;
    SDL_Image *Thumb;
}SDL_ScrollbarStyleState;

typedef struct SDL_ScrollbarStyle
{
    SDL_ScrollbarStyleState        *Normal;
    SDL_ScrollbarStyleState        *Pressed;
    SDL_ScrollbarStyleState        *Highlighted;
    SDL_ScrollbarStyleState        *Disabled;
}SDL_ScrollbarStyle;

enum
{
    SCROLLBAR_IMAGE_NORMAL_ARROW1,
    SCROLLBAR_IMAGE_NORMAL_ARROW2,
    SCROLLBAR_IMAGE_NORMAL_BAR,
    SCROLLBAR_IMAGE_NORMAL_THUMB,

    SCROLLBAR_IMAGE_PRESSED_ARROW1,
    SCROLLBAR_IMAGE_PRESSED_ARROW2,
    SCROLLBAR_IMAGE_PRESSED_BAR,
    SCROLLBAR_IMAGE_PRESSED_THUMB,

    SCROLLBAR_IMAGE_HIGHLIGHTED_ARROW1,
    SCROLLBAR_IMAGE_HIGHLIGHTED_ARROW2,
    SCROLLBAR_IMAGE_HIGHLIGHTED_BAR,
    SCROLLBAR_IMAGE_HIGHLIGHTED_THUMB,
};

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

SDL_Widget* SDL_ScrollbarNew();



int SDL_ScrollbarGetCurrentValue(SDL_Widget *widget);

void SDL_ScrollbarSetCurrentValue(SDL_Widget *widget,int CurrentValue);
int SDL_ScrollbarSetMaxValue(SDL_Widget *widget,int MaxValue);
void SDL_ScrollbarSetCallback(SDL_Widget *widget,void *callback,SDL_Widget *returnwidget);

int SDL_ScrollbarSetImage(SDL_Widget *widget,int which,SDL_Image *image);

#endif /* __SDL_SCROLLBAR_H__ */
