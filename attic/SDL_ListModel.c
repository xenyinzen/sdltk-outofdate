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
#include "SDL_ListModel.h"
#include "SDL_ModelType.h"

SDL_ModelType SDL_ListModelGetType()
{
    SDL_ModelType listmodel;
    
    listmodel = SDL_ModelTypeGet("SDL_ListModel");

    
    if(listmodel == 0)
    {
        SDL_ModelFunctions *func;
		
        listmodel            = SDL_ModelTypeRegister("SDL_ListModel",&func);
    }
    return listmodel;
}

SDL_Model *SDL_ListModelNew()
{
    SDL_Model *model;
	
    model=malloc(sizeof(SDL_Model));
    memset(model,0,sizeof(SDL_Model));

    model->Type = SDL_ListModelGetType();

    return model;
}




