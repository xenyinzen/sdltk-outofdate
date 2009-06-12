/*
  SDLTk

  Table implementation
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
#include <stdarg.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_Table.h"

void SDL_TableDraw (SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);



static void SDL_TableAddSelected(SDL_Table *table,int row);
static void SDL_TableSignalHide(SDL_Object *object,void *signaldata,void *userdata); 
static void SDL_TableSignalShow(SDL_Object *object,void *signaldata,void *userdata); 
static void SDL_TableSignalEditActivate(SDL_Object *object,void *signaldata,void *userdata); 
static void SDL_TableSignalKeyDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_TableSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_TableSignalMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_TableSignalRealize(SDL_Object *object,void *signaldata,void *userdata);

/* only valid if the model is set */
static void SDL_TableSignalAddRow(SDL_Object *object,void *signaldata,void *userdata);
#if 0
static void SDL_TableClipColumnButtons(SDL_Table *Table);
#endif
static void SDL_TableBuildViewItems(SDL_Table *Table);


SDL_WidgetType SDL_TableGetType()
{
    SDL_WidgetType table;

    table=SDL_WidgetTypeGet("SDL_Table");
    
    if(table == 0)
    {
        SDL_WidgetFunctions *func;

        table=SDL_WidgetTypeRegister("SDL_Table",&func);
        func->draw        = SDL_TableDraw;
    }
    return table;
}

SDL_Widget* SDL_TableNew()
{
    SDL_Table  *Table;
    SDL_Widget *Widget;

    Table =(SDL_Table*)malloc(sizeof(SDL_Table));
    memset(Table,0,sizeof(SDL_Table));


    Widget            = (SDL_Widget*)Table;
    Widget->Type      = SDL_TableGetType();
    Widget->Rect.x    = 0;
    Widget->Rect.y    = 0;
    Widget->Rect.w    = 0;
    Widget->Rect.h    = 0;
    Widget->flags     = WIDGET_FOCUSABLE;

    Table->bgcolor        = SDL_MapRGB(SDLTk_GetVideoSurface()->format,0x00,0x00,0x00);
    Table->fgcolor        = SDL_MapRGB(SDLTk_GetVideoSurface()->format,0x00,0xff,0xff);
    Table->sel_bg_color   = SDL_MapRGB(SDLTk_GetVideoSurface()->format,0x00,0x00,0x80);
    Table->sel_fg_color   = SDL_MapRGB(SDLTk_GetVideoSurface()->format,0x00,0xab,0x00);

    Table->next              = NULL;
    Table->RowHeight         = 20;

    Table->Column            = NULL;
    
    Table->VisibleRows       = 0;

    Table->Columns           = 0;
    Table->Rows              = 0;
    
    Table->font              = &DefaultFont;

    Table->CurrentRow        = -1;
    Table->CurrentColumn     = -1;

    Table->TablePreviousHighlightedRow = -1;
    Table->TableSelectionChanged       =  0;

    Table->Edit              = NULL;
    Table->EditCell          = NULL;


    Table->ButtonHeight      = 0;

    Table->SelectMode        = TABLE_MODE_NONE;

    Table->FirstVisibleRow   = 0;
    Table->HighlightedRow    = -1;
    Table->editcaption       = NULL;
    Table->ActiveEntry       = -1;
    Table->SelectionModel    = NULL;
    
    SDL_SignalConnect(Widget,"hide",           SDL_TableSignalHide,           Widget);
    SDL_SignalConnect(Widget,"show",           SDL_TableSignalShow,           Widget);
    SDL_SignalConnect(Widget,"realize",        SDL_TableSignalRealize,        Widget);
    SDL_SignalConnect(Widget,"keydown",        SDL_TableSignalKeyDown,        Widget);
    SDL_SignalConnect(Widget,"mousebuttondown",SDL_TableSignalMouseButtonDown,Widget);
    SDL_SignalConnect(Widget,"mousebuttonup",  SDL_TableSignalMouseButtonUp,  Widget);

    
    return (SDL_Widget*)Table;
}

void SDL_TableDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Rect R;
    /*
      widget.x + margin.left = 0
      
    */
    SDL_Table *Table=(SDL_Table*)widget;

    Table->VisibleRows = (widget->Rect.h - Table->ButtonHeight) / Table->RowHeight -3;

    /*
     * Complete redraw of the entire table
     */
    R.x = 0;
    R.y = 0;
    R.w = widget->Rect.w;
    R.h = widget->Rect.h;
    if(Table->bgcolor !=  TRANSPARANT)
        SDL_FillRect(surface,&R,Table->bgcolor);

    {
        int row_count = SDL_ViewItemCount(&Table->viewitem); /* total items */
        int cur_row;

        for(cur_row = 0;cur_row < row_count;cur_row++)
        {
            SDL_Rect r;
            SDL_ModelIndex *index;
            
            {
                SDL_Value   value;
                SDL_String *string;
                SDL_Image  *image;
                int field = 0;
                    

                for(field = 0; field < Table->Columns; field++)
                {
                    index = SDL_ModelGetIndex(Table->model,cur_row,field,NULL);              
                    if(index == NULL)
                        continue;
                    SDL_ModelGetData(index->model,index,SDL_VALUE_STRING|SDL_VALUE_IMAGE,&value);
                    
                    string = value.string;
                    image  = value.image;
                    
                    r.x = -widget->leftmargin + Table->Column[field].Width * field;
                    r.y = -widget->topmargin  + cur_row * Table->RowHeight;
                    
                    r.w = Table->Column[field].Width; 
                    r.h = Table->RowHeight;
                    
                    if(SDL_SelectionModelIsSelected(Table->SelectionModel,index))
                    {
                        SDL_FillRect(surface,&r,0x0000ff);

                        r.x = -widget->leftmargin + Table->Column[field].Width * field;
                        r.y = -widget->topmargin  + cur_row * Table->RowHeight;
                        r.w = Table->Column[field].Width; 
                        r.h = Table->RowHeight;
                    }
                    if(image)
                    {
                        r.w = image->surface->w;
                        r.h = image->surface->h;
                        
                        SDL_ImageBlitToSurface(image,surface,&r);                           
                    }
                    if(string)
                        SDL_FontDrawString(surface,Table->font,string,Table->fgcolor,&r);

                }
            }
        }
    }
}

int SDL_TableSetFont(SDL_Widget *widget,SDL_Font *font)
{
    SDL_Table *Table=(SDL_Table*)widget;
    int height;

    if(Table == NULL)
        return 0;

    Table->font = font;
    height = SDL_FontGetHeight(Table->font)+2;
    if(height > Table->RowHeight)
        Table->RowHeight = height;
    Table->VisibleRows = (widget->Rect.h - Table->ButtonHeight) / Table->RowHeight -1;

    return 1;
}

int SDL_TableSetColumns(SDL_Widget *widget,int count)
{
    int i;
    SDL_Table *Table=(SDL_Table*)widget;
    Table->Columns = count;
    if(Table->Columns)
    {
        Table->Column = calloc(sizeof(SDL_TableColumn)*Table->Columns,
                               Table->Columns);
        for(i=0;i<Table->Columns;i++)
        {
            Table->Column[i].Width  =  widget->Rect.w/Table->Columns; 
            
        }
    }
    return 1;
}


int SDL_TableEventHandler(SDL_Widget *widget,SDL_Event *event)
{
    SDL_Table *Table=(SDL_Table*)widget;
    int y;
    int i;

    Table->TablePreviousHighlightedRow = Table->HighlightedRow;
    
    switch(event->type)
    {
    case SDL_MOUSEMOTION:
        if(Table->model)
        {
            if(SDL_WidgetIsInside(widget,event->motion.x,event->motion.y))
            {
                if(event->motion.y > (widget->Rect.y + Table->ButtonHeight))
                {
                    y = widget->Rect.y + Table->ButtonHeight;

                    for(i=0;i<Table->VisibleRows;i++)
                    {
                        if(event->motion.y > y && event->motion.y < y+Table->RowHeight)
                        {
                            if(Table->TablePreviousHighlightedRow!=i)
                            {
                                /* If there is data avaible than show the row */
                                Table->TableSelectionChanged=1;
                            }   
                            Table->HighlightedRow=i;
                            break;
                        }
                        y+=Table->RowHeight;
                    }
                }
                else
                {
                    if(Table->TablePreviousHighlightedRow != -1)
                    {
                        Table->TableSelectionChanged=1;
                    }
                    Table->HighlightedRow=-1;
                }
            
            }
            else
            {
                if(Table->TablePreviousHighlightedRow != -1)
                    Table->TableSelectionChanged=1;
            
                Table->HighlightedRow=-1;
            }
            if(Table->TableSelectionChanged)
            {
                SDL_WidgetRedrawEvent(widget);
                Table->TableSelectionChanged=0;
            }
        }

        break;
    case SDL_KEYDOWN:
        switch( event->key.keysym.sym ) 
        {
        case SDLK_DOWN:
            if(SDL_WidgetHasFocus(widget))
            {
                if(Table->SelectMode == TABLE_MODE_SINGLE)
                {
                    SDL_TableAddSelected(Table,Table->CurrentRow++);	  
                    SDL_WidgetRedrawEvent(widget);
                }
#if 0
                if(Table->HighlightedRow < 0)
                    Table->HighlightedRow = 0;
                else
                {
                    if(Table->HighlightedRow >=  Table->VisibleRows -1)
                    {
                        Table->HighlightedRow = Table->VisibleRows -1;
                        if(Table->Scrollbar != NULL)
                        {
                            Table->FirstVisibleRow++;
                            SDL_ScrollbarSetCurrentValue(Table->Scrollbar,Table->FirstVisibleRow);
                        }
                    }
                    else
                    {
                        Table->HighlightedRow++;
                    }
                }
#endif
            }
            break;
        case SDLK_UP:
            if(SDL_WidgetHasFocus(widget))
            {
                if(Table->SelectMode == TABLE_MODE_SINGLE)
                {
                    SDL_TableAddSelected(Table,Table->CurrentRow--);	  
                    SDL_WidgetRedrawEvent(widget);
                }
#if 0
                if(Table->HighlightedRow < 0)
                    Table->HighlightedRow = 0;
                else
                {
                    if(Table->HighlightedRow > 0)
                        Table->HighlightedRow--;
                    else
                    {
                        if(Table->Scrollbar != NULL)
                        {
                            Table->FirstVisibleRow--;
                            SDL_ScrollbarSetCurrentValue(Table->Scrollbar,Table->FirstVisibleRow);
                        }
                    }
                }
#endif
                SDL_WidgetRedrawEvent(widget);
            }
            break;
        case SDLK_HOME:
            if(SDL_WidgetHasFocus(widget))
            {
                Table->HighlightedRow  = 0;
                Table->FirstVisibleRow = 0;
                SDL_WidgetRedrawEvent(widget);
            }
            break;
        case SDLK_END:
            if(SDL_WidgetHasFocus(widget))
            {
                Table->HighlightedRow  = Table->VisibleRows - 1;
                Table->FirstVisibleRow = Table->Rows - Table->VisibleRows - 1;
                SDL_WidgetRedrawEvent(widget);
            }
            break;
        case SDLK_PAGEUP:
            break;
        case SDLK_PAGEDOWN:
            break;
        case SDLK_RETURN:
            if(SDL_WidgetHasFocus(widget))
                SDL_SignalEmit(widget,"activate",NULL);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return 0;
}


static void SDL_TableAddSelected(SDL_Table *Table,int row)
{
    SDL_ModelIndex *rowindex;
   
    rowindex = SDL_ModelGetIndex(Table->model,row,0,NULL);
    	
    if(rowindex != NULL)
    {
        switch(Table->SelectMode)
        {
        case TABLE_MODE_SINGLE:
            SDL_SelectionModelClear(Table->SelectionModel);
            SDL_SelectionModelSelectRow(Table->SelectionModel,rowindex);
            break;
        case TABLE_MODE_MULTIPLE:
            SDL_SelectionModelSelectRow(Table->SelectionModel,rowindex);
            break;
        default:
            break;
        }
    }
}

int SDL_TableSetColumnTitle(SDL_Widget *widget,int column, char *title)
{
    SDL_Table *Table=(SDL_Table*)widget;
    Sint16 x=0;
    int newwidth;

    if(Table == NULL)
        return 0;

    if(column < 0 || column > Table->Columns)
        return 0;

            
    if(column > 0)
    {
        int i;
        for(i=0;i<column;i++)
            x+=Table->Column[i].Width;

    }
    Table->ButtonHeight = 22;
    Table->VisibleRows  = (widget->Rect.h - Table->ButtonHeight) / Table->RowHeight -1;


    newwidth=SDL_FontGetStringWidth(Table->font,title)+20;
    SDL_TableSetColumnWidth(widget,column,newwidth);


    if(Table->Column[column].Button == NULL)
    {
        SDL_Rect NewBut;
        SDL_Widget *b;

        b=SDL_ButtonNew();
        NewBut.x = widget->Rect.x + x;
        NewBut.y = widget->Rect.y;
        NewBut.w = Table->Column[column].Width;
        NewBut.h = Table->ButtonHeight;
        SDL_WidgetSetDims(b,NewBut.x,NewBut.y,NewBut.w,NewBut.h);
        Table->Column[column].Button = b;
  


    }

    SDL_ButtonSetLabel(Table->Column[column].Button,title);

    return 1;

}


static void SDL_TableSignalHide(SDL_Object *object,void *signaldata,void *userdata)
{
}

void SDL_TableSignalShow(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Table *Table=(SDL_Table*)object;

    if(Table->Column!= NULL && Table->Column[0].Button != NULL)
    {
        SDL_WidgetShow(Table->Column[0].Button);
    }
}

int SDL_TableSetEditable(SDL_Widget *widget,int value)
{
    SDL_Table *Table=(SDL_Table*)widget;
    
    if(Table == NULL)
        return 0;

    Table->Editable = value;
    return 1;
}

static void SDL_TableSignalEditActivate(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Table *Table=(SDL_Table*)object;

    if(Table->Edit)
    {
        SDL_StringSetText(&Table->EditCell->String,SDL_EditGetText(Table->Edit));

        SDL_WidgetClose(Table->Edit);
        Table->Edit     = NULL;
        Table->EditCell = NULL;
        SDL_SignalEmit((SDL_Widget*)object,"edited",NULL);
        SDL_WidgetRedrawEvent((SDL_Widget*)object);
    }
}



int SDL_TableSetCursor(SDL_Widget *widget,int row,int column)
{
    SDL_Table *Table=(SDL_Table*)widget;
    SDL_Rect RowDims;
    int i;

    if(Table != NULL && Table->Editable)
    {
        if(Table->Edit == NULL)
        {
            RowDims.w = Table->Column[column].Width;
            RowDims.x = widget->Rect.x;
            for(i=0;i<column;i++)
                RowDims.x += Table->Column[column].Width;
            RowDims.y = widget->Rect.y + Table->RowHeight * row + Table->ButtonHeight;
            RowDims.h = Table->RowHeight;

            SDL_TableEnsureRowVisible(widget,row);        
    


            Table->Edit=SDL_EditNew();
            SDL_WidgetSetDims(Table->Edit,RowDims.x,RowDims.y,RowDims.w,RowDims.h);

            
            SDL_EditSetText(Table->Edit,Table->EditCell[column].String.Data);
            
	    SDL_WidgetSetFocus(Table->Edit);
            SDL_SignalConnect(Table->Edit,"activate",SDL_TableSignalEditActivate,Table);
            SDL_WidgetRedrawEvent(widget);
        }
        
    }
    return 1;
}


int SDL_TableSetColor(SDL_Widget *widget,int row,Uint32 color)
{
    SDL_Table *Table=(SDL_Table*)widget;

    Table->bgcolor = color;
    return 1;
}

int SDL_TableSetStyle(SDL_Widget *widget,int row,Uint32 color)
{
    SDL_Table *Table=(SDL_Table*)widget;
    SDL_TableRow *tmp;
    
    if(Table)
    {
        tmp= (SDL_TableRow*)Table->model;
        while(row-- && tmp)
            tmp=tmp->Next;
	
	if(tmp)
            tmp->fgcolor=color;
    }
    return 1;
}

int SDL_TableIsVisible(SDL_Widget *widget,int row)
{
    SDL_Table *Table=(SDL_Table*)widget;

    if(Table != NULL)
    {
	if(row > Table->FirstVisibleRow &&
	   row < (Table->FirstVisibleRow + Table->VisibleRows))
            return 1;
    }
    return 0;
}

int SDL_TableEnsureRowVisible(SDL_Widget *widget,int row)
{
    SDL_Table *Table=(SDL_Table*)widget;

    if(Table != NULL)
    {
	if(SDL_TableIsVisible(widget,row) == 0)
        {
        }
    }
    return 1;
}

int SDL_TableSetColumnWidth(SDL_Widget *widget,int column,int width)
{
    int totalwidth,i;
    SDL_Table *Table=(SDL_Table*)widget;

    if(Table == NULL)
        return 0;

    if(Table->Columns <= 0)
        return 0;
        
    if(column > Table->Columns)
        return 0;
    
    totalwidth=0;
    Table->Column[column].Width=width;
    
    for(i=0;i<Table->Columns;i++)
        totalwidth += Table->Column[i].Width;

    if(totalwidth > widget->Rect.w )
    {		
        widget->rightmargin  = totalwidth - widget->Rect.w;
        SDL_SignalEmit(widget,"margins",NULL);
    }
    return 1;
    
}


int SDL_TableSetSelectionMode(SDL_Widget *widget,int mode)
{
    SDL_Table *Table=(SDL_Table*)widget;
    if(Table == NULL)
        return 0;

    Table->SelectMode = mode;
    if(mode > 0)
        Table->SelectionModel = SDL_SelectionModelNew();

    return 1;
}


static void SDL_TableSignalKeyDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Table  *Table  = (SDL_Table*)  object;
    SDL_Event  *Event  = (SDL_Event*)  signaldata;
    SDL_Widget *Widget = (SDL_Widget*) object;

    if(SDL_WidgetHasFocus(Widget))
    {
        switch(Event->key.keysym.sym ) 
        {
        case SDLK_DOWN:
            if(Table->SelectMode == TABLE_MODE_SINGLE)
            {
                SDL_TableAddSelected(Table,++Table->CurrentRow);	  
                SDL_TableEnsureRowVisible(Widget,Table->CurrentRow);        
                SDL_WidgetRedrawEvent(Widget);
            }
            break;
        case SDLK_UP:
            if(Table->SelectMode == TABLE_MODE_SINGLE)
            {
                SDL_TableAddSelected(Table,--Table->CurrentRow);	  
                SDL_TableEnsureRowVisible(Widget,Table->CurrentRow);
                SDL_WidgetRedrawEvent(Widget);

            }
            break;
        case SDLK_RETURN:
            SDL_SignalEmit(Widget,"activate",NULL);
            break;
        default:
            break;
        }
    }
}

static void SDL_TableSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Widget *Widget = (SDL_Widget*) object;
    SDL_Table  *Table  = (SDL_Table*)  object;
    SDL_Event  *Event  = (SDL_Event*)  signaldata;


    if(!SDL_WidgetIsInside(Widget,Event->motion.x,Event->motion.y))
        return;

    if(Event->button.button == 1)
    {
        if(Event->motion.y > Table->ButtonHeight)
        {
            Table->CurrentRow = (Event->motion.y - Table->ButtonHeight + Widget->topmargin ) / Table->RowHeight;

            if(Table->SelectMode > 0)
            {
                /* Add to selected */
                SDL_TableAddSelected(Table,Table->CurrentRow);
                SDL_SignalEmit(Widget,"select-row",(void*)Table->CurrentRow);
                SDL_WidgetRedrawEvent(Widget);
            }

        }
    }
}

static void SDL_TableSignalMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata)
{
}

static void SDL_TableSignalRealize(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Widget *widget = (SDL_Widget *)object;    

    if(widget->rightmargin > 0)
    {
        if(widget->rightmargin > widget->Rect.w)
            widget->rightmargin   -= widget->Rect.w;
        else
            widget->rightmargin = 0;
    }
    
    if(widget->bottommargin > 0)
    {

        if(widget->bottommargin > widget->Rect.h)
            widget->bottommargin -= widget->Rect.h;
        else
            widget->bottommargin = 0;
        
    }

    SDL_SignalObjectEmit(object,"margins",NULL);    
}

#if 0
static void SDL_TableClipColumnButtons(SDL_Table *Table)
{
    SDL_Widget *w,*b;
    int i;
    int width=0;
    
    w = (SDL_Widget*)Table;

    
    for (i=0;i < Table->Columns;i++)
    {
        if(Table->Column[i].Button != NULL)
        {
            b=Table->Column[i].Button;
#if 0
            SDL_WidgetSetDims(b,Table->Widget.Rect.x - Table->xoffset + width,b->Rect.y,b->Rect.w,b->Rect.h);
#endif
            SDL_WidgetSetClipArea(Table->Column[i].Button,w->Rect.x,w->Rect.y,w->Rect.w,w->Rect.h);
            width+=b->Rect.w;
        }
    }
}
#endif

int SDL_TableSetModel(SDL_Widget *widget,SDL_Model *model)
{
    SDL_Table *Table =(SDL_Table*)widget;
	
    Table->model = model;

    /* if a row is added to the model the widget is notified */
    SDL_SignalObjectConnect((SDL_Object*)model,"add-row", SDL_TableSignalAddRow,  widget);
    
    SDL_TableBuildViewItems(Table);
    
    return 1;
}

static void SDL_TableSignalAddRow(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_ModelInsertRowMessage *msg = (SDL_ModelInsertRowMessage *)signaldata;
    SDL_Table *Table = (SDL_Table*)userdata;

    SDL_ModelIndex *idx;
    int i;

    idx = msg->StartRow;

    if(msg->Parent == NULL)
    {
        for(i=msg->start; i<msg->end+1; i++)
        {
            SDL_ViewItemInsert(&Table->viewitem,i,1);
#if 0
            Table->viewitem.viewitem[i].index = idx;
            idx = idx->next;
#endif
        }
    }
    else
    {

    }    
}


static void SDL_TableBuildViewItems(SDL_Table *Table)
{
    int i,j,rows;
    int height=15;
    int totalheight = 0;
    int totalwidth  = 0;
    int tempwidth  = 0;

    SDL_ModelIndex *idx;
    SDL_Value value;
    SDL_Image  *image  = NULL;    
    SDL_String *string = NULL;
    SDL_Widget *widget = (SDL_Widget *)Table;

    if(Table->Columns == 0)
        SDL_TableSetColumns(widget, Table->model->FieldCount);

    rows = SDL_ModelRowCount(Table->model,NULL);

    for(i=0;i<rows;i++)
    {
        height=0;
        for(j=0;j <= Table->model->FieldCount;j++)
        {
            idx = SDL_ModelGetIndex(Table->model,i,j,NULL);
            if(idx == NULL)
                continue;
            tempwidth = 0;

            SDL_ModelGetData(idx->model,idx,SDL_VALUE_IMAGE|SDL_VALUE_STRING,&value);

            image  = value.image;
            string = value.string;
            if(image)
            {
                tempwidth += (image->surface->w + 5);

                if((image->surface->h + 5) > height)
                    height = image->surface->h + 5;
            }
            if(string)
            {
                SDL_Rect rect;
                SDL_FontCalcDims(Table->font,string,&rect);
            }
        }
        totalheight += height;
        
        if(tempwidth > totalwidth)
            totalwidth = tempwidth;
        
        SDL_ViewItemInsert(&Table->viewitem,i,1);
        Table->viewitem.viewitem[i].index = idx;


    }
    Table->RowHeight = height;


    if(totalwidth > widget->Rect.w)
        widget->rightmargin   = totalwidth  - widget->Rect.w; 
    if(totalheight > widget->Rect.h)
        widget->bottommargin = totalheight - widget->Rect.h; 

    SDL_SignalEmit((SDL_Widget*)Table,"margins",NULL);        
}
