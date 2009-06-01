/*
  SDLTk

  Model index structures
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
#ifndef __SDL_MODEL_H__
#define __SDL_MODEL_H__

struct SDL_Model;
#include "SDL_Images.h"
#include "SDL_String.h"
#include "SDL_LList.h"

typedef struct SDL_ModelIndex
{	
    /* for row browsing , the minimal form */
    SDL_LinkedListElem row_ll;

    struct SDL_Model *model;

    int row;
    int column;
}SDL_ModelIndex;

typedef struct SDL_Model
{
    int Type;
    int RowCount;
    int FieldCount;
    SDL_ModelIndex *Root;
}SDL_Model;

typedef struct SDL_ModelInsertRowMessage
{
    SDL_ModelIndex *Parent;
    int start;
    int end;
    SDL_ModelIndex *StartRow;
}SDL_ModelInsertRowMessage;

typedef struct SDL_Value
{
    unsigned int content;
    SDL_String *string;
    SDL_Image  *image;
    void       *userdata;
}SDL_Value;

#define SDL_VALUE_STRING         0x000000001
#define SDL_VALUE_IMAGE          0x000000002
#define SDL_VALUE_USER_DEFINED   0x000000004

SDL_ModelIndex *SDL_ModelGetIndex(SDL_Model *model,int row,int column,SDL_ModelIndex *parent);
int SDL_ModelGetData(SDL_Model *model,SDL_ModelIndex *index,int displayvalue,SDL_Value *value);
int SDL_ModelSetData(SDL_Model *model,SDL_ModelIndex *index,int displayvalue,SDL_Value *value);

int SDL_ModelRowCount(SDL_Model *model,SDL_ModelIndex *parent);
int SDL_ModelRemoveRows(SDL_Model *model,SDL_ModelIndex *from,SDL_ModelIndex *to,SDL_ModelIndex *parent);
int SDL_ModelHasChildren(SDL_Model *model,SDL_ModelIndex *index);

SDL_ModelIndex *SDL_ModelInsertRow(SDL_Model *model,SDL_ModelIndex *parent);
SDL_ModelIndex *SDL_ModelRowNew(SDL_Model *model);
int SDL_ModelAddRow(SDL_Model *model,SDL_ModelIndex *row,SDL_ModelIndex *parent);
SDL_ModelIndex* SDL_ModelGetChild(SDL_ModelIndex *modelindex);

#endif /* __SDL_MODEL_H__ */
