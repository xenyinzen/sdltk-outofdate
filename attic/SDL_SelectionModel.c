/*
  SDLTk

  This model represents a list data structure which can contain images and/or text
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

#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_SelectionModel.h"


SDL_SelectionModel *SDL_SelectionModelNew()
{
    SDL_SelectionModel *selectionmodel;
	
    selectionmodel=malloc(sizeof(SDL_SelectionModel));
    memset(selectionmodel,0,sizeof(SDL_SelectionModel));
    
    SDL_LListInit(&selectionmodel->Selection);

    return selectionmodel;
}

static SDL_SelectionModelItem *SDL_SelectionModelItemNew(SDL_ModelIndex *index)
{
    SDL_SelectionModelItem *item;

    item=malloc(sizeof(SDL_SelectionModelItem));
    memset(item,0,sizeof(SDL_SelectionModelItem));

    item->Index = index;

    return item;
}

void SDL_SelectionModelSelect(SDL_SelectionModel *model,SDL_ModelIndex *index)
{
    SDL_SelectionModelItem *item;

    item = SDL_SelectionModelItemNew(index);
    
    SDL_LListAddToEnd(&model->Selection,&item->ll);
}

void SDL_SelectionModelSelectRow(SDL_SelectionModel *model,SDL_ModelIndex *index)
{
    SDL_ModelIndex *idx;
    int i=0;
    int row = index->row;
    idx=index;
    
    for(i=0;i<index->model->FieldCount;i++)
    {
        SDL_SelectionModelSelect(model,idx);
        idx=SDL_ModelGetIndex(index->model,row,i,NULL);

    }
}

void SDL_SelectionModelDeSelect(SDL_SelectionModel *model,SDL_ModelIndex *index)
{


}

void SDL_SelectionModelDeSelectRow(SDL_SelectionModel *model,SDL_ModelIndex *index)
{


}

int SDL_SelectionModelIsSelected(SDL_SelectionModel *model,SDL_ModelIndex *index)
{
    SDL_SelectionModelItem *item;

    item = (SDL_SelectionModelItem*)model->Selection.first;
    
    while(item)    
    {
        if(item->Index == index)
            return 1;
        item=(SDL_SelectionModelItem*)item->ll.next;
    }
    return 0;
}


void SDL_SelectionModelClear(SDL_SelectionModel *model)
{
    SDL_LListFreeAll(&model->Selection);
}
