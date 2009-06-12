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
#ifndef __SDL_MENU_H__
#define __SDL_MENU_H__

#include "SDL_Widget.h"
#include "SDL_String.h"
#include "SDL_Font.h"

typedef struct SDL_MenuEntry
{
    SDL_Rect Rect;
    SDL_String string;

    SDL_Widget *Child;
    SDL_Widget *Root;

    struct SDL_MenuEntry *next;
}SDL_MenuEntry;

typedef struct SDL_Menu
{
    SDL_Widget Widget;
    SDL_Widget *Target;
	
    SDL_Font *Font;
    Uint32   bgcolor;
    Uint16 Width;
    Uint16 Height;

    SDL_MenuEntry *Selected;
    SDL_MenuEntry *Entries;
    
}SDL_Menu;


SDL_Widget* SDL_MenuNew();


SDL_MenuEntry *SDL_MenuAppend(SDL_Widget *widget,char *txt);
SDL_MenuEntry *SDL_MenuSubAppend(SDL_Widget *widget,SDL_MenuEntry *Parent,char *txt);

void SDL_MenuAttach(SDL_Widget *widget,SDL_Widget *target);
void SDL_MenuPopup(SDL_Widget *widget);

#endif /* __SDL_MENU_H__ */
