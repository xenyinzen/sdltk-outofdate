/*
  SDLTk

  Support for tree and table drawing 
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
#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_ViewItem.h"

int SDL_ViewItemInsert(SDL_ViewItemList *vil,int start,int count)
{
    SDL_ViewItem *l;

    vil->count += count;

    l = malloc(vil->count*sizeof(SDL_ViewItem));
    memset(l,0,vil->count*sizeof(SDL_ViewItem));

    if(vil->viewitem)
    {
        memcpy(l,vil->viewitem,start*sizeof(SDL_ViewItem));
    }

    if(count+start<vil->count)
    {
        memcpy(&l[start+count],
               &vil->viewitem[start],
               (vil->count-count-start)*sizeof(SDL_ViewItem));
    }
    free(vil->viewitem);
    vil->viewitem=l;
    return 0;
}


int SDL_ViewItemRemove(SDL_ViewItemList *vil,int start,int count)
{
    SDL_ViewItem *l;
    vil->count -= count;
    l=malloc(vil->count*sizeof(SDL_ViewItem));
    memset(l,0,vil->count*sizeof(SDL_ViewItem));
    if(start > 0)
    {
        memcpy(l,vil->viewitem,(start)*sizeof(SDL_ViewItem));
        if(start+count<vil->count)
        {
            memcpy(&l[start],
                   &vil->viewitem[start+count],
                   (vil->count-start)*sizeof(SDL_ViewItem));
        }
	
    }
    else
    {
        memcpy(l,vil->viewitem+count,vil->count*sizeof(SDL_ViewItem));
    }	
    free(vil->viewitem);
    vil->viewitem=l;
    return 0;
}

SDL_ModelIndex *SDL_ViewItemGetModelIndex(SDL_ViewItemList *vil,int row)
{
    if(row >= 0 && row < vil->count)
        return vil->viewitem[row].index;
    return NULL;
}

int SDL_ViewItemCount(SDL_ViewItemList *vil)
{
    return vil->count;
}
