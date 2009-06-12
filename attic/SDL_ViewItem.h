/*
  SDLTk

  Support for tree and table viewing 
  Copyright (C) 2004-2005 John Beuving (john.beuving@beatforce.org)

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

#ifndef __SDL_VIEWITEM_H__
#define __SDL_VIEWITEM_H__

#include "SDL_Model.h"

typedef struct SDL_ViewItem
{
    SDL_ModelIndex *index;
    int expanded;
    int height;
    int level;
}SDL_ViewItem;

typedef struct SDL_ViewItemList
{
    SDL_ViewItem *viewitem;
    int count;
}SDL_ViewItemList;

int SDL_ViewItemInsert(SDL_ViewItemList *vil,int start,int count);
int SDL_ViewItemRemove(SDL_ViewItemList *vil,int start,int count);
SDL_ModelIndex *SDL_ViewItemGetModelIndex(SDL_ViewItemList *vil,int row);
int SDL_ViewItemCount(SDL_ViewItemList *vil);

#endif /* __SDL_VIEWITEM_H__ */
