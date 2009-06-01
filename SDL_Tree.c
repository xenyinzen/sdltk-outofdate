/*
  SDLTk

  Tree widget 
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
#include <stdarg.h>
#include <malloc.h>
#include <string.h>


#include "SDLTk.h"
#include "SDL_Tree.h"

#define FONTOFFSET 0

static void SDL_TreeCollapse(SDL_Tree *Tree,int number);
static void SDL_TreeExpand(SDL_Tree *Tree,int number);

static void SDL_TreeDrawExpander(SDL_Surface *surface,
                                 SDL_Tree *Tree,
                                 SDL_ModelIndex *Item,
                                 int row);

static void SDL_TreeDrawLines(SDL_Surface *surface,
                              SDL_Tree *Tree,
                              void *Item,int row);


static void SDL_TreeSignalMouseButtonDown(SDL_Object *Widget,void *signaldata,void *userdata);
static void SDL_TreeSignalRealize(SDL_Object *widget,void *signaldata,void *userdata);
void SDL_TreeLayout(SDL_Tree *tree,int rowstart);
static void SDL_TreeDrawCell(SDL_Tree *tree,SDL_Surface *surface,SDL_Rect *rect,SDL_ModelIndex *index);
void SDL_TreeInsertRow(SDL_Object *object,void *data,void *userdata);
static void SDL_TreeBuildViewItems(SDL_Tree *Tree);

static void        SDL_TreeDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);

SDL_WidgetType SDL_TreeGetType()
{
    SDL_WidgetType tree;
    
    tree = SDL_WidgetTypeGet("SDL_Tree");
    
    if(tree == 0)
    {
        SDL_WidgetFunctions *func;

        tree=SDL_WidgetTypeRegister("SDL_Tree",&func);
        func->draw        = SDL_TreeDraw;
		
    }
    return tree;
}

SDL_Widget* SDL_TreeNew()
{
    SDL_Widget *Widget;
    SDL_Tree   *Tree;

    Tree=(SDL_Tree*)malloc(sizeof(SDL_Tree));
    memset(Tree,0,sizeof(SDL_Tree));
    Widget=(SDL_Widget*)Tree;
    
    Widget->Type    = SDL_TreeGetType();
    Widget->Rect.x  = 0;
    Widget->Rect.y  = 0;
    Widget->Rect.w  = 0;
    Widget->Rect.h  = 0;

    Tree->VisibleItems       = 0; /* Number of items to draw */
    Tree->FirstVisible = 0; /* First item to draw      */


    Tree->Font    = &DefaultFont;

    Tree->fgcolor = WHITE;
    Tree->bgcolor = BLACK;

    Tree->model      = NULL;

    SDL_SignalConnect(Widget,"mousebuttondown",SDL_TreeSignalMouseButtonDown,Widget);
    SDL_SignalConnect(Widget,"realize",        SDL_TreeSignalRealize,        Widget);
    return (SDL_Widget*)Tree;
}



void SDL_TreeDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Tree *Tree=(SDL_Tree*)widget;
    int currentrow;
    SDL_Rect r;
    int row=0;
    int n=0;

    if(Tree->bgcolor != TRANSPARANT)
    {
        r.x = 0;
        r.y = 0;
        r.w = widget->Rect.w;
        r.h = widget->Rect.h;
        SDL_FillRect(surface,NULL,Tree->bgcolor);
    }

    n = SDL_ViewItemCount(&Tree->viewitem); /* total items */

    for( currentrow = 0; currentrow < n; currentrow++)
    {
        int j = currentrow;
        int height = SDL_FontGetHeight(Tree->Font) + FONTOFFSET;
        SDL_ModelIndex *index = SDL_ViewItemGetModelIndex(&Tree->viewitem,j);              
		
        if(index)
        {
            SDL_TreeDrawExpander(surface,Tree,index,currentrow);
            SDL_TreeDrawLines(surface,Tree,index,currentrow);

            if(Tree->Font)
            {
                r.x = 11 * Tree->viewitem.viewitem[(currentrow-row)].level + 22;
                r.h = height +1;
                r.y = (currentrow-row) * r.h + 1 - widget->topmargin;
                r.w = widget->Rect.w;
                
#if 0
                
                if(Item == Tree->Selected)
                    SDL_FontDrawStringWindow(window,Tree->Font,&Item->Label,Tree->fgcolor,&r);
                else
#endif
                    SDL_TreeDrawCell(Tree,surface,&r,index);
            }
        }
    }
}

static void SDL_TreeDrawCell(SDL_Tree *tree,SDL_Surface *surface,SDL_Rect *rect,SDL_ModelIndex *index)
{
    SDL_String *string;
    SDL_Value   value;
    if(index)
    {
        SDL_ModelGetData(index->model,index,SDL_VALUE_STRING,&value);
        string=value.string;
        SDL_FontDrawString(surface,tree->Font,string,tree->fgcolor,rect);
    }
}


static void SDL_TreeDrawExpander(SDL_Surface *surface,
                                 SDL_Tree *Tree,
                                 SDL_ModelIndex *Item,
                                 int row
    )
{
    SDL_Widget *widget = (SDL_Widget*)Tree;

    Sint16 height = SDL_FontGetHeight(Tree->Font) + FONTOFFSET + 1;
    
    Sint16 x = 20 * Tree->viewitem.viewitem[row].level;
    Sint16 y = height * row + ((height/2)-4) - widget->topmargin;

#if 0
    if(Item->collapsed == 2)
        return;
#endif

    if(x >  Tree->Widget.Rect.w + 8)
        return;
    
    if(y >  Tree->Widget.Rect.h + 8)
        return;

    boxColor(surface,x,y,x+8,y+8,Tree->bgcolor);
    rectangleColor(surface,x,y,x+8,y+8,Tree->fgcolor);
    SDL_DrawLine (surface,x + 2, y + 4, x + 6, y + 4,Tree->fgcolor);
    if(Tree->viewitem.viewitem[row].expanded == 0)
        SDL_DrawLine (surface,x + 4, y + 2, x + 4, y + 6,Tree->fgcolor);
}

static void SDL_TreeDrawLines(SDL_Surface *surface,
                              SDL_Tree *Tree,
                              void *Item,int row)
{

    Sint16 x = Tree->Widget.Rect.x + 10 * 1;

    void *Parent = NULL;
#if 0
    /* The small line on the bottom of the expander */
    if(Tree->viewitem.viewitem[row].expanded == 1)
        SDL_DrawLineWindow (window,x + 4, y + height - 1, x + 4, y + height   ,Tree->fgcolor);
#endif

    if(Parent == NULL) /* return when it is the root node */
        return;

    /* draw point of the lines is 10 pixels in front of the expander */
    x -= 10;

#if 0

    /* Vertical line */
    if(Item->next )
        SDL_DrawLineWindow (window,x + 4, y    , x + 4, y + height   ,Tree->fgcolor);
    else
        SDL_DrawLineWindow (window,x + 4, y    , x + 4, y + height/2,Tree->fgcolor);
    

    /* Horizontal line */
    if(Item->collapsed == 2) /*If there is no expander make the line a little longer */
        SDL_DrawLineWindow (window,x + 5, y + height/2, x + 15, y + height/2,Tree->fgcolor);
    else
        SDL_DrawLineWindow (window,x + 5, y + height/2, x + 9, y + height/2,Tree->fgcolor);

    /* Parent line */
    while(Parent)
    {
        x-=10;
        if(Parent->parent != NULL && Parent->parent->collapsed == 0)
        {
            if(Parent->next) /* Parent vertical line */
                SDL_DrawLineWindow (window,x + 4, y    , x + 4, y + height ,Tree->fgcolor);
        }
        Parent=Parent->parent;
    }
#endif
}

int SDL_TreeSetFont(SDL_Widget *widget,SDL_Font *font)
{
    SDL_Tree *Tree=(SDL_Tree*)widget;
    int h;

    Tree->Font=font;
    h=SDL_FontGetHeight(Tree->Font) + FONTOFFSET;
    if(h)

    {
        Tree->VisibleItems=widget->Rect.h / h;
    }
    return 1;
}


static void SDL_TreeSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Widget *widget = (SDL_Widget*)object;
    SDL_Tree *Tree=(SDL_Tree*)widget;
    SDL_Event *event=(SDL_Event*)signaldata;

    if(SDL_WidgetIsInside(widget,event->motion.x,event->motion.y))
    {
        if(event->button.button == 1)
        {
            int rowheight = SDL_FontGetHeight(Tree->Font)+ FONTOFFSET + 1;
            int y  = event->motion.y - widget->Rect.y  + widget->topmargin;
            int row;

            if(event->motion.x < widget->Rect.x + widget->Rect.w)
                if(event->motion.x > (widget->Rect.x + (widget->Rect.w - 9)))
                    return;

            row = y / rowheight;
                
            if(Tree->viewitem.viewitem[row].expanded == 1)
                SDL_TreeCollapse(Tree,row);
            else
                SDL_TreeExpand(Tree,row);
            
            SDL_SignalEmit(widget,"clicked",SDL_ViewItemGetModelIndex(&Tree->viewitem,row));
            SDL_WidgetRedrawEvent(widget);
        }
        if(event->button.button == 4) /* mousehweel down */
        {
        }
        if(event->button.button == 5) /* Mousewheel up */
        {
        }
            
    }
}



void *SDL_TreeGetSelectedItem(void *tree)
{

    return NULL;

}





#if 0
static void* SDL_TreeGetItem(SDL_Tree *Tree,int *number)
{
    if( *number >= Tree->viewitem.count)
        return NULL;
    return NULL;
}
#endif





void SDL_TreeRemove(SDL_Tree *tree,int number)
{
    int i,j;
    SDL_ViewItem *l;
    SDL_ModelIndex *ind = SDL_ViewItemGetModelIndex(&tree->viewitem,number);
    int total = SDL_ModelRowCount(tree->model,ind);
    l=malloc((tree->viewitem.count-total)*sizeof(SDL_ViewItem));
    memset(l,0,(tree->viewitem.count-total)*sizeof(SDL_ViewItem));

    for(i=0,j=0;i<tree->viewitem.count;i++)
    {
        if(i< (number+1) || i > (number+1+total))
        {
            l[j].height = tree->viewitem.viewitem[i].height;
            l[j].index  = tree->viewitem.viewitem[i].index;
            l[j].expanded  = tree->viewitem.viewitem[i].expanded;

            j++;
        }
    }

    tree->viewitem.count -= total;

    free(tree->viewitem.viewitem);
    tree->viewitem.viewitem=l;


}

static void SDL_TreeExpand(SDL_Tree *Tree,int number)
{
    SDL_Widget *widget = (SDL_Widget*)Tree;
    int n;

    if(number >= Tree->viewitem.count)
        return;

    Tree->viewitem.viewitem[number].expanded = 1;

    if(SDL_ModelHasChildren(Tree->viewitem.viewitem[number].index->model,Tree->viewitem.viewitem[number].index))
    {
        SDL_ViewItemInsert(&Tree->viewitem,number+1,1);

        Tree->viewitem.viewitem[number+1].index = SDL_ModelGetChild(Tree->viewitem.viewitem[number].index);
        Tree->viewitem.viewitem[number+1].level = Tree->viewitem.viewitem[number].level+1;
    }
    
    n = SDL_ViewItemCount(&Tree->viewitem); /* total items */
    widget->bottommargin = n*(SDL_FontGetHeight(Tree->Font) + FONTOFFSET + 1)  - widget->Rect.h; 
    SDL_SignalEmit(widget,"margins",NULL);    
}



static void SDL_TreeCollapse(SDL_Tree *Tree,int number)
{
    SDL_Widget *widget = (SDL_Widget*)Tree;
    SDL_ModelIndex *parent  = SDL_ViewItemGetModelIndex(&Tree->viewitem,number);

    int count=0;
    int n;
    if(SDL_ModelHasChildren(Tree->model,parent))
    {
        count=SDL_ModelRowCount(Tree->model,SDL_ModelGetChild(parent));
        if(count > 0)
        {
            SDL_ViewItemRemove(&Tree->viewitem,number+1,count);
        }
    }
    Tree->viewitem.viewitem[number].expanded = 0;

    n = SDL_ViewItemCount(&Tree->viewitem); /* total items */
    widget->bottommargin = n*(SDL_FontGetHeight(Tree->Font) + FONTOFFSET + 1)  - widget->Rect.h; 
    SDL_SignalEmit(widget,"margins",NULL);    
}


int SDL_TreeSetColor(SDL_Widget *widget,int row,Uint32 color)
{
    SDL_Tree *Tree=(SDL_Tree*)widget;
    
    if(Tree == NULL)
        return 0;

    Tree->bgcolor = color;
    return 1;
}



static void SDL_TreeSignalRealize(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Tree *Tree =(SDL_Tree*)object;
    SDL_Widget *widget = (SDL_Widget *)object;
    int h;
    int      n = SDL_ViewItemCount(&Tree->viewitem); /* total items */

    h = SDL_FontGetHeight(Tree->Font) + FONTOFFSET  + 1;
    if(h)
    {
        Tree->VisibleItems = Tree->Widget.Rect.h / h;
    }
    n = SDL_ViewItemCount(&Tree->viewitem); /* total items */
    widget->bottommargin = n*(SDL_FontGetHeight(Tree->Font) + FONTOFFSET + 1)  - widget->Rect.h; 
    SDL_SignalObjectEmit(object,"margins",NULL);    
}

int SDL_TreeSetModel(SDL_Widget *widget,SDL_Model *model)
{
    SDL_Tree *Tree =(SDL_Tree*)widget;
    int n;

    Tree->model = model;

    /* if a row is inserted in the model the widget is notified */
    SDL_SignalObjectConnect((SDL_Object*)model,"insert-row",SDL_TreeInsertRow,(void*)widget);
    
    SDL_TreeBuildViewItems(Tree);

    n = SDL_ViewItemCount(&Tree->viewitem); /* total items */
    widget->bottommargin = n*(SDL_FontGetHeight(Tree->Font) + FONTOFFSET + 1)  - widget->Rect.h; 
    SDL_SignalEmit(widget,"margins",NULL);    

    return 1;
}



void SDL_TreeItemAt(void *list)
{

}



void SDL_TreeLayout(SDL_Tree *tree,int rowstart)
{
    SDL_ModelIndex *current;
    SDL_ModelIndex *parent  = SDL_ViewItemGetModelIndex(&tree->viewitem,rowstart);

    int count=0;

    if(SDL_ModelHasChildren(tree->model,parent))
        count=SDL_ModelRowCount(tree->model,parent);


    if(rowstart == -1)
    {
        tree->viewitem.count    = count;
        if(count > 0)
        {
            tree->viewitem.viewitem = realloc(tree->viewitem.viewitem,tree->viewitem.count*sizeof(SDL_ViewItem));
            memset(tree->viewitem.viewitem,0,tree->viewitem.count*sizeof(SDL_ViewItem));
        }
        else
        {
            free(tree->viewitem.viewitem);
            tree->viewitem.viewitem = NULL;
        }
    }
    else if(count >  0)
    {
        SDL_ViewItemInsert(&tree->viewitem,rowstart,count);
    }
    { 
        int first = rowstart + 1;
        int hidden = 0;
        int last = 0;
        int j;
        for(j=first;j<first+count;++j)
        {
            current=SDL_ModelGetIndex(tree->model,j-first,0,parent);
            last=j-hidden;
            tree->viewitem.viewitem[last].index = current;
        }
    }


}

void SDL_TreeInsertRow(SDL_Object *object,void *data,void *userdata)
{
    SDL_Tree *Tree=userdata;
    SDL_ModelInsertRowMessage *msg = data;
    SDL_ModelIndex *idx;
    int i;

    idx=msg->StartRow;

    if(msg->Parent == NULL)
    {
        for(i=msg->start;i<msg->end+1;i++)
        {
            SDL_ViewItemInsert(&Tree->viewitem,i,1);
            Tree->viewitem.viewitem[i].index = idx;
#if 0
            idx=idx->next;
#endif
        }
    }
    else
    {

    }
}

void SDL_TreeRemoveRow(SDL_Tree *tree,SDL_ModelIndex *parent,int row,int count)
{
    if(parent == NULL)
    {
        SDL_ViewItemRemove(&tree->viewitem,row,1);
    }
}

static void SDL_TreeBuildViewItems(SDL_Tree *Tree)
{
    int i,j,rows;
    int height=15;
    int totalheight = 0;
    int totalwidth  = 0;
    int tempwidth  = 0;

    SDL_ModelIndex *idx;
    SDL_Value value;
    SDL_Image *image = NULL;
    SDL_Widget *widget = (SDL_Widget *)Tree;

#if 0
    if(Tree->Columns == 0)
        SDL_TreeSetColumns(widget, Tree->model->FieldCount);
#endif

    rows = SDL_ModelRowCount(Tree->model,NULL);

    for(i=0;i<rows;i++)
    {
        for(j=0;j <= idx->model->FieldCount;j++)
        {
            idx = SDL_ModelGetIndex(Tree->model,i,0,NULL);
            tempwidth = 0;
            
            SDL_ModelGetData(idx->model,idx,SDL_VALUE_IMAGE,&value);
            image = (SDL_Image*)value.image;
            
            if(image)
            {
                tempwidth += (image->surface->w + 5);

                if((image->surface->h + 5) > height)
                    height = image->surface->h + 5;
            }

        }
        
        if(tempwidth > totalwidth)
            totalwidth = tempwidth;
        
        SDL_ViewItemInsert(&Tree->viewitem,i,1);
        Tree->viewitem.viewitem[i].index = idx;

        if(image)
            totalheight += (image->surface->h + 5);

    }
#if 0
    Tree->RowHeight = height;
#endif

    if(totalwidth > widget->Rect.w)
        widget->rightmargin   = totalwidth  - widget->Rect.w; 
    if(totalheight > widget->Rect.h)
        widget->bottommargin = totalheight - widget->Rect.h; 

    SDL_SignalEmit((SDL_Widget*)Tree,"margins",NULL);        
}
