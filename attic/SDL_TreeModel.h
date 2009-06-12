/*
  SDLTk

  Tree Model index structures
  Copyright (C) 2005-2006 John Beuving (john.beuving@beatforce.org)

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

#ifndef __SDL_TREEMODEL_H__
#define __SDL_TREEMODEL_H__

#include "SDL_Images.h"
#include "SDL_Model.h"
#include "SDL_Tree.h"
#include "SDL_LList.h"

typedef struct SDL_TreeModelIndex
{	
    struct SDL_ModelIndex index;

    /* for column browsing , tables*/
    SDL_LinkedListElem column_ll;

    /* for parent child browsing, table trees */
    struct SDL_ModelIndex *child;
    struct SDL_ModelIndex *parent;

    SDL_String *String;
    SDL_Image  *Image;
    int Field;
}SDL_TreeModelIndex;


SDL_ModelIndex *SDL_TreeModelInsertRow(SDL_Model *model,SDL_ModelIndex *parent);
SDL_Model *SDL_TreeModelNew(int rows,int columns);

#endif /* __SDL_TREEMODEL_H__ */


