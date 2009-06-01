/*
  SDLTk

  Tree widget
  Copyright (C) 2004-2006 John Beuving (john.beuving@beatforce.org)

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

#ifndef __SDL_TREE_H__
#define __SDL_TREE_H__

#include "SDL_Widget.h"
#include "SDL_String.h"
#include "SDL_Font.h"
#include "SDL_Model.h"
#include "SDL_ViewItem.h"


typedef struct SDL_Tree
{
    SDL_Widget Widget;
    SDL_Font  *Font;
    
    Uint32  bgcolor;
    Uint32  fgcolor;

    int VisibleItems;

    int FirstVisible;
    SDL_ViewItemList viewitem;

    SDL_Model *model;
}SDL_Tree;

SDL_Widget* SDL_TreeNew();

int         SDL_TreeProperties(SDL_Widget *widget,int feature,va_list list);
int         SDL_TreeEventHandler(SDL_Widget *widget,SDL_Event *event);

void *SDL_TreeGetSelectedItem(void *tree);


int SDL_TreeSetModel(SDL_Widget *widget,SDL_Model *model);

#endif /* __SDL_TREE_H__ */
