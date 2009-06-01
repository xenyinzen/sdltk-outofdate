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
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_ProgressBar.h"

/* Prototypes for Internal functions */
static void ProgressBar_DrawStripe(SDL_Surface *dest,SDL_ProgressBar *ProgressBar,int line,Uint32 color);
static void ProgressBar_CurrentValue(SDL_ProgressBar * ProgressBar);


/* signal handler prototype */
static void SDL_ProgressBarEventMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ProgressBarEventMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ProgressBarEventMouseMotion(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ProgressBarDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);


SDL_WidgetType SDL_ProgressBarGetType()
{
    SDL_WidgetType progressbar;

    progressbar = SDL_WidgetTypeGet("SDL_ProgressBar");
    
    if(progressbar == 0)
    {
        SDL_WidgetFunctions *func;
		
        progressbar = SDL_WidgetTypeRegister("SDL_ProgressBar",&func);
        func->draw = SDL_ProgressBarDraw;

    }
    return progressbar;
}

SDL_Widget* SDL_ProgressBarNew()
{
    SDL_Widget      *widget;
    SDL_ProgressBar *progressbar;
    
    progressbar=(SDL_ProgressBar*)malloc(sizeof(SDL_ProgressBar));
    memset(progressbar,0,sizeof(SDL_ProgressBar));
    widget=(SDL_Widget*)progressbar;

    widget->Type    = SDL_ProgressBarGetType();
    widget->Rect.x  = 0;
    widget->Rect.y  = 0;
    widget->Rect.w  = 0;
    widget->Rect.h  = 0;

    SDL_SignalConnect(widget,"mousebuttondown",SDL_ProgressBarEventMouseButtonDown,widget);
    SDL_SignalConnect(widget,"mousebuttonup",SDL_ProgressBarEventMouseButtonUp,widget);
    SDL_SignalConnect(widget,"mousemotion",SDL_ProgressBarEventMouseMotion,widget);


    progressbar->color1  = 0x007f80;
    progressbar->color2  = 0xa8cbf1;

    /* Default range settings */
    progressbar->MaxValue      = 100;
    progressbar->MinValue      = 0;
    progressbar->CurrentValue  = 0;

    /* Used for drawing */
    progressbar->CurrentLine   = 0;
    progressbar->State         = PROGRESSBAR_NORMAL;

    return (SDL_Widget*)progressbar;
}

static void SDL_ProgressBarDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_ProgressBar *ProgressBar=(SDL_ProgressBar*)widget;
    int line;
    int highlight=0;
    int Maxline=0;
    SDL_Rect R;
    
    if(widget->Rect.h > widget->Rect.w)
        ProgressBar->Orientation = VERTICAL;
    else
        ProgressBar->Orientation = HORIZONTAL;

    if(ProgressBar->Orientation      == VERTICAL)
        Maxline = widget->Rect.h;
    else if(ProgressBar->Orientation == HORIZONTAL)
        Maxline = widget->Rect.w;

    R.x = 0;
    R.y = 0;
    R.w = widget->Rect.w;
    R.h = widget->Rect.h;

    SDL_FillRect(surface,&R,BLACK);

    for( line=0; line < Maxline ; line+=2)
    {
        highlight= (line < ProgressBar->CurrentLine);
        if(highlight)
            ProgressBar_DrawStripe(surface,ProgressBar,line,ProgressBar->color2);
        else
            ProgressBar_DrawStripe(surface,ProgressBar,line,ProgressBar->color1);
    }
}

int SDL_ProgressBarSetCurrentValue(SDL_Widget *widget,int value)
{
    SDL_ProgressBar *ProgressBar=(SDL_ProgressBar*)widget;

    if(ProgressBar->State == PROGRESSBAR_DRAG)
        return 0;
    if(ProgressBar->CurrentValue != value )
    {
        ProgressBar->CurrentValue = value; 
        if(ProgressBar->Orientation == VERTICAL)
        {
            ProgressBar->CurrentLine  = (value * widget->Rect.h) / 
                (ProgressBar->MaxValue - ProgressBar->MinValue); 
        }
        else
        {
            ProgressBar->CurrentLine  = (value * widget->Rect.w) 
                / (ProgressBar->MaxValue - ProgressBar->MinValue); 
        }
        
        SDL_WidgetRedrawEvent(widget);
    }
    return 0;
}

int SDL_ProgressBarSetMaxValue(SDL_Widget *widget, int max)
{
    SDL_ProgressBar *ProgressBar=(SDL_ProgressBar*)widget;

    if(ProgressBar == NULL)
        return 0;

    ProgressBar->MaxValue = max;

    return 1;
}

/* Internal Helper functions */


void ProgressBar_DrawStripe(SDL_Surface *dest,SDL_ProgressBar *ProgressBar,int line,Uint32 color)
{
    SDL_Widget *widget=(SDL_Widget*)ProgressBar;
    SDL_Rect rect;

    if(ProgressBar->Orientation == VERTICAL)
    {
        rect.x = 0;
        rect.y = widget->Rect.h - line;
        rect.w = widget->Rect.w;
        rect.h = 1;
    }
    else if(ProgressBar->Orientation == HORIZONTAL)
    {
        rect.x = line;
        rect.y = 0;
        rect.w = 1;
        rect.h = widget->Rect.h;
    }
    SDL_FillRect(dest,&rect,color);
}


static void ProgressBar_CurrentValue(SDL_ProgressBar * ProgressBar)
{
    SDL_Widget *widget=(SDL_Widget*)ProgressBar;
    int BarRange=1; /* in pixels */

    if(ProgressBar->CurrentLine < 0)
        ProgressBar->CurrentLine = 0;

    if(ProgressBar->Orientation == HORIZONTAL)
    {
        BarRange = widget->Rect.w;
    }
    else if(ProgressBar->Orientation == VERTICAL)
    {
        BarRange = widget->Rect.h;
    }
    if(ProgressBar->CurrentLine > BarRange)
        ProgressBar->CurrentLine = BarRange;
    
    ProgressBar->CurrentValue  = (ProgressBar->MaxValue - ProgressBar->MinValue)*ProgressBar->CurrentLine;
    ProgressBar->CurrentValue /= BarRange;
    ProgressBar->CurrentValue += ProgressBar->MinValue;
}

double SDL_ProgressBarGetCurrentValue(SDL_Widget *widget)
{
    SDL_ProgressBar *ProgressBar=(SDL_ProgressBar*)widget;
    return ProgressBar->CurrentValue;
}


int SDL_ProgressBarGetState(SDL_Widget *widget)
{
    SDL_ProgressBar *ProgressBar=(SDL_ProgressBar*)widget;
    if(ProgressBar == NULL)
        return -1;
  
    return ProgressBar->State;
}


static void SDL_ProgressBarEventMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_ProgressBar *ProgressBar=(SDL_ProgressBar*)object;
    SDL_Widget *widget=(SDL_Widget*)object;
    SDL_Event *event=(SDL_Event*)signaldata;

    if(SDL_WidgetIsInside(widget,event->motion.x,event->motion.y))
    {
        if(ProgressBar->Orientation == HORIZONTAL)
        {
            ProgressBar->State = PROGRESSBAR_DRAG;
            ProgressBar->CurrentLine = event->motion.x - widget->Rect.x;
            ProgressBar_CurrentValue(ProgressBar);
            SDL_WidgetRedrawEvent(widget);
        }
    }

}

static void SDL_ProgressBarEventMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_ProgressBar *ProgressBar=(SDL_ProgressBar*)object;
    SDL_Widget *widget=(SDL_Widget*)object;
    SDL_Event *event=(SDL_Event*)signaldata;

    if(ProgressBar->State == PROGRESSBAR_DRAG ||
       SDL_WidgetIsInside(widget,event->motion.x,event->motion.y))
    {
        if(ProgressBar->Orientation == VERTICAL)
        {
            ProgressBar->CurrentLine = event->motion.y - widget->Rect.y;
            ProgressBar_CurrentValue(ProgressBar);
            SDL_WidgetRedrawEvent(widget);
        }
        else if(ProgressBar->Orientation == HORIZONTAL)
        {
            ProgressBar->CurrentLine = event->motion.x - widget->Rect.x;
            ProgressBar_CurrentValue(ProgressBar);
            SDL_WidgetRedrawEvent(widget);
        }
        ProgressBar->State=PROGRESSBAR_NORMAL;
        SDL_SignalEmit(widget,"clicked",NULL);
    }

}


static void SDL_ProgressBarEventMouseMotion(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_ProgressBar *ProgressBar=(SDL_ProgressBar*)object;
    SDL_Widget *widget=(SDL_Widget*)object;
    SDL_Event *event=(SDL_Event*)signaldata;

    if(ProgressBar->Orientation == HORIZONTAL)
    {
        if(ProgressBar->State==PROGRESSBAR_DRAG)
        {
            ProgressBar->CurrentLine = event->motion.x - widget->Rect.x;
            ProgressBar_CurrentValue(ProgressBar);
            SDL_WidgetRedrawEvent(widget);
        }
    }

}
