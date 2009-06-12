/*
  SDLTk

  Scroll area implementation, controlling widget placement
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
#include "SDL_ScrollArea.h"

#define SCROLLSIZE 16

static void SDL_ScrollAreaSignalMargins(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ScrollAreaSignalHorChanged(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ScrollAreaSignalVerChanged(SDL_Object *object,void *signaldata,void *userdata);

void SDL_ScrollAreaNew(SDL_Widget *child)
{
    SDL_ScrollArea  *ScrollArea;

    ScrollArea =(SDL_ScrollArea*)malloc(sizeof(SDL_ScrollArea));
    
    ScrollArea->VerticalScrollbar   = NULL;
    ScrollArea->HorizontalScrollbar = NULL;

    ScrollArea->Child               = child;

    SDL_SignalConnect(child,"margins",SDL_ScrollAreaSignalMargins,ScrollArea);
}

static void SDL_ScrollAreaSignalMargins(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Widget *ClientWidget   = (SDL_Widget*)object;
    SDL_ScrollArea *ScrollArea = (SDL_ScrollArea*)userdata;

    if(ClientWidget->Rect.w <= 0 || ClientWidget->Rect.h <=0)
        return;

    if(ClientWidget->rightmargin > 0)
    {
        if(ScrollArea->HorizontalScrollbar == NULL)
        {
            ClientWidget->Rect.h -= SCROLLSIZE;
            ScrollArea->HorizontalScrollbar = SDL_ScrollbarNew();
            SDL_WidgetSetDims(ScrollArea->HorizontalScrollbar,ClientWidget->Rect.x,ClientWidget->Rect.y + ClientWidget->Rect.h,
                              ClientWidget->Rect.w,SCROLLSIZE);
            SDL_ScrollbarSetMaxValue(ScrollArea->HorizontalScrollbar,ClientWidget->rightmargin);

            SDL_SignalConnect(ScrollArea->HorizontalScrollbar,"value-changed",SDL_ScrollAreaSignalHorChanged,ClientWidget);
            SDL_WidgetShow(ScrollArea->HorizontalScrollbar);
        } 
        else
        {
            SDL_WidgetSetDims(ScrollArea->HorizontalScrollbar,ClientWidget->Rect.x,ClientWidget->Rect.y + ClientWidget->Rect.h,
                              ClientWidget->Rect.w,SCROLLSIZE);
            SDL_ScrollbarSetMaxValue(ScrollArea->HorizontalScrollbar,ClientWidget->rightmargin);
        }
    }

    if(ClientWidget->bottommargin > 0)
    {
        if(ScrollArea->VerticalScrollbar == NULL)
        {
            ClientWidget->Rect.w -= SCROLLSIZE; 
            ScrollArea->VerticalScrollbar = SDL_ScrollbarNew();
            SDL_WidgetSetDims(ScrollArea->VerticalScrollbar,ClientWidget->Rect.x + ClientWidget->Rect.w,ClientWidget->Rect.y,
                              SCROLLSIZE,ClientWidget->Rect.h);
            SDL_ScrollbarSetMaxValue(ScrollArea->VerticalScrollbar,ClientWidget->bottommargin);
            SDL_SignalConnect(ScrollArea->VerticalScrollbar,"value-changed",SDL_ScrollAreaSignalVerChanged,ClientWidget);
            SDL_WidgetShow(ScrollArea->VerticalScrollbar);
        } 
        else
        {
            SDL_WidgetSetDims(ScrollArea->VerticalScrollbar,ClientWidget->Rect.x + ClientWidget->Rect.w,ClientWidget->Rect.y,
                              SCROLLSIZE,ClientWidget->Rect.h);
            SDL_ScrollbarSetMaxValue(ScrollArea->VerticalScrollbar,ClientWidget->bottommargin);
        }
    }
}

static void SDL_ScrollAreaSignalHorChanged(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Widget *ClientWidget         = (SDL_Widget*)userdata;
    SDL_Widget *Widget               = (SDL_Widget*)object;
    int value;

    value=SDL_ScrollbarGetCurrentValue(Widget);
    
    ClientWidget->leftmargin  = value;
    SDL_WidgetRedrawEvent(ClientWidget);
}


static void SDL_ScrollAreaSignalVerChanged(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Widget *ClientWidget         = (SDL_Widget*)userdata;
    SDL_Widget *Widget               = (SDL_Widget*)object;
    int value;

    value=SDL_ScrollbarGetCurrentValue(Widget);
    ClientWidget->topmargin  = value;
    SDL_WidgetRedrawEvent(ClientWidget);
}
