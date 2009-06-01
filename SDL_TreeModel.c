/*
  SDLTk

  This model represents a data structure which can contain images and/or text
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

#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_TreeModel.h"
#include "SDL_ModelType.h"

static int SDL_TreeModelGetData(SDL_Model *model,SDL_ModelIndex *index,int displayvalue,SDL_Value *value);
static int SDL_TreeModelSetData(SDL_Model *model,SDL_ModelIndex *index,int displayvalue,SDL_Value *value);
static int SDL_TreeModelHasChildren(SDL_Model *model,SDL_ModelIndex *index);
static SDL_ModelIndex *SDL_TreeModelGetIndex(SDL_Model *model,int row,int column,SDL_ModelIndex *parent);
static int SDL_TreeModelRowCount(SDL_Model *model,SDL_ModelIndex *parent);
static int SDL_TreeModelAddRow(SDL_Model *model,SDL_ModelIndex *parent,SDL_ModelIndex *row);
static SDL_TreeModelIndex *SDL_TreeModelNewIndex(SDL_Model *model);
static SDL_ModelIndex* SDL_TreeModelGetChild(SDL_ModelIndex *modelindex);

SDL_ModelType SDL_TreeModelGetType()
{
    SDL_ModelType treemodel;
    
    treemodel = SDL_ModelTypeGet("SDL_TreeModel");

    
    if(treemodel == 0)
    {
        SDL_ModelFunctions *func;
		
        treemodel            = SDL_ModelTypeRegister("SDL_TreeModel",&func);

        func->getdata        = SDL_TreeModelGetData;
        func->setdata        = SDL_TreeModelSetData;
        func->insertrow      = SDL_TreeModelInsertRow;
        func->haschildren    = SDL_TreeModelHasChildren;
        func->index          = SDL_TreeModelGetIndex;
        func->rowcount       = SDL_TreeModelRowCount;
        func->addrow         = SDL_TreeModelAddRow;
        func->getchild       = SDL_TreeModelGetChild;
    }
    return treemodel;
}

void SDL_TreeModelAddColumn(SDL_Model *model,SDL_TreeModelIndex *row,SDL_TreeModelIndex *column)
{
    SDL_TreeModelIndex    *index;
    index = row;
    
    while(index->column_ll.next)
        index = (SDL_TreeModelIndex*)index->column_ll.next;

    index->column_ll.next = (SDL_LinkedListElem*)column;
}

SDL_Model *SDL_TreeModelNew()
{
    SDL_Model *model;

    model=malloc(sizeof(SDL_Model));
    memset(model,0,sizeof(SDL_Model));

    model->Type = SDL_TreeModelGetType();
    model->RowCount   = 0;
    model->FieldCount = 0;

    return model;
}


static int SDL_TreeModelGetData(SDL_Model *model,SDL_ModelIndex *index,int displayvalue,SDL_Value *value)
{
    SDL_TreeModelIndex    *tindex = (SDL_TreeModelIndex *)index;

    memset(value,0,sizeof(SDL_Value));

    if(displayvalue & SDL_VALUE_STRING)
    {
        value->string   = tindex->String;
        value->content |= SDL_VALUE_STRING;
    }
    if(displayvalue & SDL_VALUE_IMAGE)
    {
        value->image    = tindex->Image;
        value->content |= SDL_VALUE_IMAGE;
    }
    return 1;

}

/* store some things */
static int SDL_TreeModelSetData(SDL_Model *model,SDL_ModelIndex *index,int displayvalue,SDL_Value *value)
{
    SDL_TreeModelIndex *tindex = (SDL_TreeModelIndex*)index;

    if(index == NULL)
        return 0;

    if(displayvalue & SDL_VALUE_STRING)
    {
        tindex->String = value->string;
    }
    if(displayvalue & SDL_VALUE_IMAGE)
    {
        tindex->Image   = value->image;
    }
    return 1;
}

/* check for children, can be used in tree views */
static int SDL_TreeModelHasChildren(SDL_Model *model,SDL_ModelIndex *index)
{
    SDL_TreeModelIndex *tindex = (SDL_TreeModelIndex*)index;
    if(tindex && tindex->child)
        return 1;
    return 0;
}


SDL_ModelIndex *SDL_TreeModelInsertRow(SDL_Model *model,SDL_ModelIndex *parent)
{
    SDL_TreeModelIndex *tparent = (SDL_TreeModelIndex*)parent;
    SDL_TreeModelIndex *index;
    SDL_TreeModelIndex *walker;

    SDL_ModelInsertRowMessage ir;

    index=malloc(sizeof(SDL_TreeModelIndex));
    memset(index,0,sizeof(SDL_TreeModelIndex));

    index->index.model  = model;

    if(model->Root == NULL)
    {
        model->Root = (SDL_ModelIndex*)index;
        
        ir.Parent   = NULL;
        ir.start    = 0;
        ir.end      = 0;
        ir.StartRow = (SDL_ModelIndex*)index;
    }
    else if(tparent)
    {
        if(tparent->child)
        {
            int i=1;
            walker = (SDL_TreeModelIndex*)tparent->child;
            while(walker->index.row_ll.next)
            {
                walker = (SDL_TreeModelIndex*)walker->index.row_ll.next;
                i++;
            }
            walker->index.row_ll.next  = (SDL_LinkedListElem*)index;
            index->parent              = (SDL_ModelIndex*)parent;
            ir.Parent = parent;
            ir.start = i;
            ir.end   = i;
            ir.StartRow = (SDL_ModelIndex*)index;

        }
        else
        {
            tparent->child = (SDL_ModelIndex*)index;
            index->parent = (SDL_ModelIndex*)parent;
            ir.Parent = (SDL_ModelIndex*)parent;
            ir.start = 0;
            ir.end   = 0;
            ir.StartRow = (SDL_ModelIndex*)index;

        }
		
    }
    else
    {
        int i=1;

        walker = (SDL_TreeModelIndex*)model->Root;

        while(walker->index.row_ll.next)
        {
            walker = (SDL_TreeModelIndex*) walker->index.row_ll.next;
            i++;
        }
        walker->index.row_ll.next = (SDL_LinkedListElem*)index;
        ir.Parent = NULL;
        ir.start  = i;
        ir.end    = i;
        ir.StartRow = (SDL_ModelIndex*) index;

    }
    SDL_SignalObjectEmit((SDL_Object*)model,"insert-row",&ir);
    
    return (SDL_ModelIndex*)index;
}


static SDL_ModelIndex *SDL_TreeRowAt(SDL_Model *model,int row,SDL_ModelIndex *parent)
{
    SDL_ModelIndex *index=NULL;

    if(parent == NULL)
    {
        if(model->Root == NULL)
        {
            model->Root = (SDL_ModelIndex*)SDL_TreeModelNewIndex(model);
        }
        index = model->Root;
    }
    else
    {
        index = ((SDL_TreeModelIndex*)parent)->child;
        if(index == NULL)
        {
            index = ((SDL_TreeModelIndex*)parent)->child = (SDL_ModelIndex*) SDL_TreeModelNewIndex(model);
        }
            
    }

    while(row--)
    {
        index=(SDL_ModelIndex*)index->row_ll.next;
        if(index == NULL)
        {
            index = (SDL_ModelIndex*)SDL_TreeModelNewIndex(model);
            SDL_TreeModelAddRow(model,(SDL_ModelIndex*)index,NULL);

        }
    }
    return index;
    
}

static SDL_ModelIndex *SDL_TreeColumnAt(SDL_Model *model,int column,SDL_ModelIndex *row)
{
    SDL_TreeModelIndex *index = (SDL_TreeModelIndex*)row;

    while(column-- && index)
    {
        index=(SDL_TreeModelIndex*)index->column_ll.next;
        if(index == NULL)
        {
            index = SDL_TreeModelNewIndex(model);
            SDL_TreeModelAddColumn(model,(SDL_TreeModelIndex*)row,index);
       }
    }
    return (SDL_ModelIndex*)index;
}

static SDL_ModelIndex *SDL_TreeModelGetIndex(SDL_Model *model,int row,int column,SDL_ModelIndex *parent)
{
    SDL_ModelIndex *index;

    index = SDL_TreeRowAt(model,row,parent);

    index = SDL_TreeColumnAt(model,column,index);

    if(model->FieldCount < column)
        model->FieldCount = column;
    if(index)
    {
        index->row    = row;
        index->column = column;
    }
    return index;
}

static int SDL_TreeModelAddRow(SDL_Model *model,SDL_ModelIndex *row,SDL_ModelIndex *parent)
{
    SDL_TreeModelIndex *tparent = (SDL_TreeModelIndex*)parent;
    SDL_TreeModelIndex *index = (SDL_TreeModelIndex*)row;
    SDL_TreeModelIndex *walker;

    SDL_ModelInsertRowMessage ir;

    
    if(model->Root == NULL)
    {
        model->Root = (SDL_ModelIndex*)index;
        
        ir.Parent   = NULL;
        ir.start    = 0;
        ir.end      = 0;
        ir.StartRow = (SDL_ModelIndex*)index;
    }
    else if(tparent)
    {
        if(tparent->child)
        {
            int i=1;
            walker = (SDL_TreeModelIndex*)tparent->child;
            while(walker->index.row_ll.next)
            {
                walker = (SDL_TreeModelIndex*)walker->index.row_ll.next;
                i++;
            }
            walker->index.row_ll.next  = (SDL_LinkedListElem*)index;
            index->parent              = parent;
            ir.Parent = parent;
            ir.start = i;
            ir.end   = i;
            ir.StartRow = (SDL_ModelIndex*)index;

        }
        else
        {
            tparent->child = (SDL_ModelIndex*)index;
            index->parent  = parent;
            ir.Parent = (SDL_ModelIndex*)parent;
            ir.start = 0;
            ir.end   = 0;
            ir.StartRow = (SDL_ModelIndex*)index;

        }
		
    }
    else
    {
        int i=1;
        walker = (SDL_TreeModelIndex*)model->Root;

        while(walker->index.row_ll.next)
        {
            walker = (SDL_TreeModelIndex*) walker->index.row_ll.next;
            i++;
        }
        walker->index.row_ll.next = (SDL_LinkedListElem*)index;
        ir.Parent = NULL;
        ir.start  = i;
        ir.end    = i;
        ir.StartRow = (SDL_ModelIndex*) index;

    }
/*    SDL_SignalObjectEmit((SDL_Object*)model,"add-row",&ir);*/
    return 1;
}



int SDL_TreeModelRowCount(SDL_Model *model,SDL_ModelIndex *parent)
{
    int count = 0;
    SDL_ModelIndex *tmp;

    if(parent)
        tmp=parent;
    else
        tmp=model->Root;

    while(tmp)
    {
        count++;
        tmp = (SDL_ModelIndex*)tmp->row_ll.next;
    }
    return count;
}


static SDL_TreeModelIndex *SDL_TreeModelNewIndex(SDL_Model *model)
{
    SDL_TreeModelIndex *index;
    
    index = malloc(sizeof(SDL_TreeModelIndex));
    memset(index,0,sizeof(SDL_TreeModelIndex));

    index->index.model  = model;


    return  index;
}

static SDL_ModelIndex* SDL_TreeModelGetChild(SDL_ModelIndex *modelindex)
{
    SDL_TreeModelIndex *index = (SDL_TreeModelIndex*)modelindex;

    return index->child;
}
