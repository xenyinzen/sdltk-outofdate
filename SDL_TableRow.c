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
#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_TableRow.h"


void SDL_TableRowDraw(SDL_Window *window,SDL_Table *Table,SDL_TableRow *Row, int row)
{
    SDL_Widget *Widget=(SDL_Widget*)Table;
    SDL_Rect RowDims;
    SDL_Rect la;
    int column;
    unsigned int color;


    if(Row == NULL)
        return;


    RowDims.w = Widget->Rect.w;
    RowDims.x = Widget->Rect.x;
    RowDims.y = Table->ButtonHeight + Widget->Rect.y + Table->RowHeight * row; 
    RowDims.h = Table->RowHeight;

    
    if(RowDims.y > Widget->Rect.y + Widget->Rect.h)
    {

        return;
    }
#if 0
    if(RowDims.y + RowDims.h > Widget->Rect.y + Widget->Rect.h)
    {
        printf("SDL_TableDrawRow cannot draw outside table %d\n",row);
        return;
    }
#endif
	if(Row->Cell == NULL)
		printf("Huh");

	if (Row && Row->Cell)
    {
        SDL_Rect rowbg;
        rowbg.x = Widget->Rect.x;
        rowbg.y = Table->ButtonHeight + Widget->Rect.y + Table->RowHeight * row; 
        rowbg.w = Widget->Rect.w;
        rowbg.h = Table->RowHeight;

        if(Row->bgcolor != TRANSPARANT)
            SDL_FillRectWindow(window,&rowbg,Row->bgcolor);	

        for(column=0; column<Table->Columns; column++)
        {         
            la.x = RowDims.x + 1;
            la.y = RowDims.y + 1;
            la.w = Table->Column[column].Width - 2;
            la.h = RowDims.h - 2;

#if 0 
            /* Color the individual cells */
			SDL_FillRectWindow(window,&la,SDL_MapRGB(window->surface->format,255,155,155));
#endif       
            if(SDL_StringLength(&Row->Cell[column].String))
            {
                 color = Row->fgcolor;
 

                SDL_FontDrawStringWindow(window,Table->font,&Row->Cell[column].String,color,&la);
            }
            RowDims.x +=Table->Column[column].Width;
        }
    }
}


struct SDL_TableRow *SDL_TableRowNew(SDL_Table *Table)
{
    SDL_TableRow *row;

    row=malloc(sizeof(SDL_TableRow));
    memset(row,0,sizeof(SDL_TableRow));
    row->Cell = calloc(sizeof(SDL_TableCell)*Table->Columns,Table->Columns);
    row->fgcolor = Table->fgcolor;
    row->bgcolor = Table->bgcolor;
    row->Next = NULL;
        

    return row;
}


int SDL_TableRowSetColor(struct SDL_TableRow *row,Uint32 color)
{
    row->fgcolor = color;
    return 1;
}
