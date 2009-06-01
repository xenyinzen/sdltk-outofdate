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

#ifndef __SDL_TABLE_H__
#define __SDL_TABLE_H__

#include "SDL.h"

#include "SDL_Widget.h"
#include "SDL_TableCell.h"
#include "SDL_TableRow.h"
#include "SDL_Font.h"
#include "SDL_Edit.h"
#include "SDL_Model.h"
#include "SDL_ViewItem.h"

typedef enum E_TableMode
{
    TABLE_MODE_NONE,             /* No selection possible             */
    TABLE_MODE_SINGLE_ROW,       /* Only one row can be selected      */
    TABLE_MODE_MULTIPLE_ROWS,    /* Multiple entries can be selected  */
    TABLE_MODE_SINGLE_CELL,      /* Only one cell can be selected     */
    TABLE_MODE_MULTIPLE_CELLS    /* Multiple cells can be selected    */
}E_TableMode;




typedef struct SDL_TableColumn
{
    Uint16 Width;
    char *Title;
    SDL_Widget *Button;
}SDL_TableColumn;

typedef struct SDL_List
{
    struct SDL_List *next;
    struct SDL_List *prev;
    void *data;
}SDL_List;
  
typedef struct SDL_Table
{
    SDL_Widget          Widget;
    SDL_Rect            ButtonBar;
    SDL_Font            *font;
    char                *caption;    
  

    Uint32   bgcolor;
    Uint32   fgcolor;
    Uint32   sel_bg_color;
    Uint32   sel_fg_color;


    int Rows;
    int Columns;


    SDL_TableColumn   *Column;

    SDL_mutex         *RowDataLock;
    SDL_Model         *model;
    
    int RowHeight;

    int VisibleRows;

    int FirstVisibleRow;
    int HighlightedRow; /* Where the mouse is on */
    int ActiveEntry;

    int CurrentRow;
    int CurrentColumn;

    E_TableMode SelectMode;

    int TablePreviousHighlightedRow;
    int TableSelectionChanged;

    SDL_Widget *Edit;

    SDL_TableCell *EditCell;

    char *editcaption;

    int ButtonHeight;



    int Editable;

    struct SDL_Table    *next;
    SDL_ViewItemList     viewitem;
    SDL_SelectionModel  *SelectionModel;
}SDL_Table;

SDL_Widget* SDL_TableNew();



/* Modification functions */
int SDL_TableSetColumns(SDL_Widget *widget,int count);
SDL_TableRow *SDL_TableAddRow(SDL_Widget *Widget,char *Titles[]);
SDL_TableRow  *SDL_TableGetRow(SDL_Widget *widget,int row);
int SDL_TableSetColumnTitle(SDL_Widget *widget,int column, char *title);
int SDL_TableSetEditable(SDL_Widget *widget,int value);
int SDL_TableSetCursor(SDL_Widget *widget,int row,int column);
int SDL_TableSetFont(SDL_Widget *widget,SDL_Font *font);
int SDL_TableSetText(SDL_Widget *widget,int row,int column,char *text);
int SDL_TableSetStyle(SDL_Widget *widget,int row,Uint32 color);
int SDL_TableEnsureRowVisible(SDL_Widget *widget,int row);
int SDL_TableIsVisible(SDL_Widget *widget,int row);
int SDL_TableSetColumnWidth(SDL_Widget *widget,int column,int width);
int SDL_TableSetSelectionMode(SDL_Widget *widget,int mode);
int SDL_TableSetModel(SDL_Widget *widget,SDL_Model *model);


#endif /* __SDL_TABLE_H__ */
