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

#ifndef __SDL_TAB_H__
#define __SDL_TAB_H__

#include "SDL_Widget.h"
#include "SDL_Button.h"
#include "SDL_Edit.h"
#include "SDL_Font.h"

typedef struct SDL_TabList
{
    char                    *caption;
    int index;
    SDL_Rect                *rect;
    SDL_Widget              *Child;
    int                     draw;
    struct SDL_TabList      *next;
    struct SDL_TabList      *prev;
}SDL_TabList;


typedef struct SDL_TabImage
{
    SDL_Surface *surface;
    int slopewidth;
}SDL_TabImage;

typedef struct SDL_Tab
{
    SDL_Widget        Widget;
    SDL_TabImage      *normal;
    SDL_TabImage      *highlighted;

    SDL_TabList       *tabs;
    SDL_TabList       *hl;

    SDL_Widget        *edit;
    SDL_Widget        *ArrowLeft;
    SDL_Widget        *ArrowRight;
    SDL_Font          *font;
    int NoOfTabs;

    int startx;
    int maxstartx;
    int doesntfit;
    int             min_width;  /* the minimum width a tab may have  */
    int             min_height; /* the minimum height a tab may have */
    int             overlay;    /* overlay of tabs in pixels         */
    Uint32          bgcolor;
    Uint32          fgcolor;

}SDL_Tab;


SDL_Widget *SDL_NotebookNew();



/* modifier functions */
int SDL_NotebookSetFont(SDL_Widget *widget,SDL_Font *font);
int SDL_NotebookSetCurrentTab(SDL_Widget *widget,int tabnr);
int SDL_NotebookGetCurrentPage(SDL_Widget *widget);
SDL_Widget *SDL_NotebookGetChildWidget(SDL_Widget *widget);

int SDL_NotebookAppendTab(SDL_Widget *parent,SDL_Widget *child,char *caption);
int SDL_NotebookClear(SDL_Widget *widget);

enum
{
    BG_COLOR,
    FG_COLOR
};
int SDL_NotebookSetColor(SDL_Widget *widget,int type,Uint32 color);
#endif /* __SDL_TAB_H__ */






