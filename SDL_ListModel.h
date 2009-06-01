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

#ifndef __SDL_LISTMODEL_H__
#define __SDL_LISTMODEL_H__

#include "SDL_Images.h"
#include "SDL_Model.h"
#include "SDL_LList.h"


typedef struct SDL_ListModelIndex
{	
    struct SDL_ModelIndex index;
    SDL_String *String;
}SDL_ListModelIndex;



SDL_Model *SDL_ListModelNew();


#endif /* __SDL_LISTMODEL_H__ */




