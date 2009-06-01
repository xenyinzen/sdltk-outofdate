/*
  Beatforce/SDLTk

  one line to give the program's name and an idea of what it does.
  Copyright (C) 2003-2005 John Beuving (john.beuving@beatforce.org)

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
#ifndef __SDL_MODELTYPE_H__
#define __SDL_MODELTYPE_H__

#include "SDL_Model.h"

typedef int SDL_ModelType;

typedef int (*T_Model_GetData)(SDL_Model *model,SDL_ModelIndex *index,int displayvariant,SDL_Value *value);
typedef int (*T_Model_SetData)(SDL_Model *model,SDL_ModelIndex *index,int displayvariant,SDL_Value *value);
typedef int (*T_Model_HasChildren)(SDL_Model *model,SDL_ModelIndex *index);
typedef int (*T_Model_RowCount)(SDL_Model *model,SDL_ModelIndex *parent);
typedef SDL_ModelIndex *(*T_Model_Index)(SDL_Model *model,int row,int column,SDL_ModelIndex *parent);
typedef SDL_ModelIndex *(*T_Model_InsertRow)(SDL_Model *model,SDL_ModelIndex *parent);
typedef int (*T_Model_AddRow)(SDL_Model *model,SDL_ModelIndex *row,SDL_ModelIndex *parent);
typedef SDL_ModelIndex* (*T_Model_RowNew)(SDL_Model *model);
typedef SDL_ModelIndex* (*T_Model_GetChild)(SDL_ModelIndex *modelindex);

typedef struct SDL_ModelFunctions
{
    T_Model_GetData         getdata;
    T_Model_SetData         setdata;
    T_Model_HasChildren     haschildren;
    T_Model_Index           index;
    T_Model_RowCount        rowcount;
    T_Model_InsertRow       insertrow;
    T_Model_AddRow          addrow;
    T_Model_RowNew          rownew;
    T_Model_GetChild        getchild;
}SDL_ModelFunctions;

struct SDL_ModelTypeListItem
{
    char *id;
    SDL_ModelFunctions func;
};


int SDL_ModelTypeInit();
SDL_ModelFunctions *SDL_ModelTypeGetFunctions(SDL_ModelType type);
SDL_ModelType SDL_ModelTypeGet(char *id);
SDL_ModelType SDL_ModelTypeRegister(char *id,SDL_ModelFunctions **func);

#endif /* __SDL_MODELTYPE_H__ */

