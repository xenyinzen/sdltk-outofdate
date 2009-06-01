/*
  SDLTk

  The model object represents a multi dimensional data structure wrapper
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
#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_Model.h"

SDL_ModelIndex *SDL_ModelGetIndex(SDL_Model *model,int row,int column,SDL_ModelIndex *parent)
{
    SDL_ModelFunctions *func=NULL;
    if(model)	
    {
        func=SDL_ModelTypeGetFunctions(model->Type);
        if(func && func->index)
            return func->index(model,row,column,parent);
    }
    return 0;

}

int SDL_ModelGetData(SDL_Model *model,SDL_ModelIndex *index,int displayvalue,SDL_Value *value)
{
    SDL_ModelFunctions *func=NULL;
    if(model)	
    {
        func=SDL_ModelTypeGetFunctions(model->Type);
        if(func && func->index)
            return func->getdata(model,index,displayvalue,value);
    }
    return 1;
}

SDL_ModelIndex *SDL_ModelRowNew(SDL_Model *model)
{
    SDL_ModelFunctions *func=NULL;
    if(model)	
    {
        func=SDL_ModelTypeGetFunctions(model->Type);
        if(func && func->rownew)
            return func->rownew(model);
    }
    return NULL;
}

int SDL_ModelRowCount(SDL_Model *model,SDL_ModelIndex *parent)
{
    SDL_ModelFunctions *func=NULL;
    if(model)	
    {
        func=SDL_ModelTypeGetFunctions(model->Type);
        if(func && func->rowcount)
            return func->rowcount(model,parent);
    }
    return 0;
}

int SDL_ModelRemoveRows(SDL_Model *model,SDL_ModelIndex *from,SDL_ModelIndex *to,SDL_ModelIndex *parent)
{
    return 0;
}

int SDL_ModelSetData(SDL_Model *model,SDL_ModelIndex *index,int displayvalue,SDL_Value *value)
{
    SDL_ModelFunctions *func=NULL;

    if(model)	
    {
        func=SDL_ModelTypeGetFunctions(model->Type);
        if(func && func->setdata)
            return func->setdata(model,index,displayvalue,value);
    }
    return 0;

}

int SDL_ModelHasChildren(SDL_Model *model,SDL_ModelIndex *index)
{
    SDL_ModelFunctions *func=NULL;

    if(model)	
    {
        func=SDL_ModelTypeGetFunctions(model->Type);
        if(func && func->haschildren)
            return func->haschildren(model,index);
    }
    return 0;
}

int SDL_ModelAddRow(SDL_Model *model,SDL_ModelIndex *row,SDL_ModelIndex *parent)
{
    SDL_ModelFunctions *func=NULL;

    if(model)	
    {
        func=SDL_ModelTypeGetFunctions(model->Type);
        if(func && func->addrow)
            return func->addrow(model,row,parent);
    }
    return 0;
}

SDL_ModelIndex *SDL_ModelInsertRow(SDL_Model *model,SDL_ModelIndex *parent)
{
    SDL_ModelFunctions *func=NULL;

    if(model)	
    {
        func=SDL_ModelTypeGetFunctions(model->Type);
        if(func && func->insertrow)
            return func->insertrow(model,parent);
    }
    return 0;
}

SDL_ModelIndex *SDL_ModelIndexNew(SDL_Model *model,int row,int column,int usersize)
{
    SDL_ModelIndex *i;

    i=malloc(sizeof(SDL_ModelIndex)+usersize);
    memset(i,0,sizeof(SDL_ModelIndex)+usersize);

    i->model   = model;
    i->row     = row;
    i->column  = column;

    return i;
}


SDL_ModelIndex* SDL_ModelGetChild(SDL_ModelIndex *modelindex)
{
    SDL_ModelFunctions *func=NULL;

    if(modelindex->model)	
    {
        func=SDL_ModelTypeGetFunctions(modelindex->model->Type);
        if(func && func->getchild)
            return func->getchild(modelindex);
    }
    return NULL;
}

