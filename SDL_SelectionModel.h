/*
  SDLTk

  List model index structures
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

#ifndef __SDL_SELECTIONMODEL_H__
#define __SDL_SELECTIONMODEL_H__

#include "SDL_Images.h"
#include "SDL_Model.h"
#include "SDL_LList.h"

typedef struct SDL_SelectionModel
{
    SDL_LinkedList Selection;
}SDL_SelectionModel;

typedef struct SDL_SelectionModelItem
{
    SDL_LinkedListElem ll;
    SDL_ModelIndex *Index;
}SDL_SelectionModelItem;

SDL_SelectionModel *SDL_SelectionModelNew();
void SDL_SelectionModelSelect(SDL_SelectionModel *model,SDL_ModelIndex *index);
void SDL_SelectionModelSelectRow(SDL_SelectionModel *model,SDL_ModelIndex *index);

void SDL_SelectionModelDeSelect(SDL_SelectionModel *model,SDL_ModelIndex *index);
void SDL_SelectionModelDeSelectRow(SDL_SelectionModel *model,SDL_ModelIndex *index);

int SDL_SelectionModelIsSelected(SDL_SelectionModel *model,SDL_ModelIndex *index);
void SDL_SelectionModelClear(SDL_SelectionModel *model);
SDL_LinkedList *SDL_SelectionModelGetList(SDL_SelectionModel *model);

#endif /* __SDL_SELECTIONMODEL_H__ */




