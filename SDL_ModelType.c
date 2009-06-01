/*
  SDLTk

  one line to give the program's name and an idea of what it does.
  Copyright (C) 2003-2006 John Beuving (john.beuving@beatforce.org)

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
#include <string.h>

#define MAX_MODEL_TYPE 10

#include "SDL_ModelType.h"

struct SDL_ModelTypeListItem ModelTypeList[MAX_MODEL_TYPE];

int SDL_ModelTypeInit()
{
    int i;
    
    for(i=0;i<MAX_MODEL_TYPE;i++)
    {
        memset(&ModelTypeList[i],0,sizeof(struct SDL_ModelTypeListItem));
    }
    return 1;
}


SDL_ModelType SDL_ModelTypeGet(char *id)
{
    int i;

    for(i=1;i<MAX_MODEL_TYPE;i++)
    {
        if(ModelTypeList[i].id != NULL)
        {
            if(!strcmp(ModelTypeList[i].id,id))
                return i;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

SDL_ModelType SDL_ModelTypeRegister(char *id,SDL_ModelFunctions **func)
{
    int i;

    for(i=1;i<MAX_MODEL_TYPE;i++)
    {
        if(ModelTypeList[i].id != NULL)
        {
            if(!strcmp(ModelTypeList[i].id,id))
            {
                *func=&ModelTypeList[i].func;
                return i;
            }
        }
        else
        {
            ModelTypeList[i].id=id;
            *func=&ModelTypeList[i].func;
            return i;
        }
    }
    return 0;
}


SDL_ModelFunctions *SDL_ModelTypeGetFunctions(SDL_ModelType type)
{

    return &ModelTypeList[type].func;
}
