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

#ifndef __SDL_LABEL_H__
#define __SDL_LABEL_H__

#include "SDLTk.h"

enum 
{
	TOPLEFT,
	TOPCENTER,
	TOPRIGHT,
	CENTERLEFT,
	CENTER,
	CENTERRIGHT,
	BOTTOMLEFT,
	BOTTOMCENTER,
	BOTTOMRIGHT
};

typedef struct SDL_Label
{
    SDL_Widget Widget;
    SDL_Font *Font;
    int height;

    SDL_TimerID RedrawTimer;
    
    Uint32  bgcolor;
    Uint32  fgcolor;

    int Visible;
    int offset;
    int increase;
    int Alignment;

    int Pattern;

    SDL_String *Caption;
}SDL_Label;


enum LabelPattern
{
    LABEL_NORMAL,
    LABEL_BOUNCE,
    LABEL_SCROLL_LEFT,
    LABEL_SCROLL_RIGHT,
}LabelPattern;

SDL_Widget* SDL_LabelNew();
void SDL_LabelClose(SDL_Widget *widget);

int         SDL_LabelSetColor(SDL_Widget *widget,int which,Uint32 color);

int  SDL_LabelSetAlignment (SDL_Widget *widget,int Alignment);
int  SDL_LabelSetFontHeight(SDL_Widget *widget,int height);
int  SDL_LabelSetFont      (SDL_Widget *widget,SDL_Font *Font);
int  SDL_LabelSetText      (SDL_Widget *widget,char *text);

SDL_String *SDL_LabelGetText(SDL_Widget *widget);

#endif /* __SDL_LABEL_H__ */
