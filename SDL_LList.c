/*
  SDLTk

  Implementation of linked list functionality
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
#include <stdio.h>
#include <malloc.h>

#include "SDL_LList.h"

int SDL_LListInit(SDL_LinkedList *list)
{
    list->first = NULL;
    list->last  = NULL;
    return 1;
}

int SDL_LListIsEmpty(SDL_LinkedList *list)
{
    return (list->first == NULL);
}

int SDL_LListAddToEnd(SDL_LinkedList *list,SDL_LinkedListElem *elem)
{
    elem->next = NULL;
    elem->prev = list->last;

    if(list->last != NULL)
        list->last->next = elem;
    else
        list->first = elem;
	
    list->last = elem;

    return 1;
}

int SDL_LListRemove(SDL_LinkedList *list,SDL_LinkedListElem *elem)
{
    if(!list || !elem) {
    	fprintf(stderr, "NULL list or elem value.\n");
    	return 0;
    }
    
    if(elem->prev != NULL)
        elem->prev->next = elem->next;
    else
        list->first = elem->next;
    
    if(elem->next != NULL)
        elem->next->prev = elem->prev;
    else
        list->last = elem->prev;

    return 1;
}


void SDL_LListFreeAll(SDL_LinkedList *list)
{
    SDL_LinkedListElem *elem = list->first;
    SDL_LinkedListElem *next;
    
    while(elem)
    {
        next = elem->next;
        free(elem);
        elem=next;
    }
    
    SDL_LListInit(list);
}
