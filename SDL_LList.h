/*
  SDLTk

  Definition of a linked list and it's elements
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

#ifndef __SDL_LLIST_H__
#define __SDL_LLIST_H__

typedef struct SDL_LinkedListElem
{
    struct SDL_LinkedListElem *next;
    struct SDL_LinkedListElem *prev;
}SDL_LinkedListElem;

typedef struct SDL_LinkedList
{
    struct SDL_LinkedListElem *first;
    struct SDL_LinkedListElem *last;
}SDL_LinkedList;

int SDL_LListInit(SDL_LinkedList *list);
int SDL_LListIsEmpty(SDL_LinkedList *list);
int SDL_LListAddToEnd(SDL_LinkedList *list,SDL_LinkedListElem *elem);
int SDL_LListRemove(SDL_LinkedList *list,SDL_LinkedListElem *elem);
void SDL_LListFreeAll(SDL_LinkedList *list);

#endif /* __SDL_LLIST_H__ */
