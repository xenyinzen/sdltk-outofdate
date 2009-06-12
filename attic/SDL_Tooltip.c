/*
  SDLTk

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
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_Tooltip.h"



static void SDL_TooltipSignalMouseMotion(SDL_Object *Object,void *signaldata,void *userdata);
static void SDL_TooltipSignalHide(SDL_Object *Object,void *signaldata,void *userdata);

SDL_WidgetType SDL_TooltipGetType()
{
    return SDL_WidgetTypeGet("SDL_Tooltip");
}


SDL_Widget* SDL_TooltipCreate(SDL_Widget *parent, char *text)
{
    SDL_Widget    *widget;
    SDL_Tooltip   *tooltip;

    tooltip=(SDL_Tooltip*) malloc(sizeof(SDL_Tooltip));
    memset(tooltip,0,sizeof(SDL_Tooltip));

    widget=(SDL_Widget*)tooltip;

    widget->Type      = SDL_TooltipGetType();
    widget->Rect.x    = 0;
    widget->Rect.y    = 0;
    widget->Rect.w    = 10;
    widget->Rect.h    = 10;
    widget->flags     = 0;
  
    tooltip->String.Data = SDLTk_strdup(text);
    tooltip->Parent = parent;
    tooltip->Font   = &DefaultFont;

    tooltip->Lines  = 1;

    SDL_SignalConnect(parent,"mousemotion",SDL_TooltipSignalMouseMotion,widget);
    SDL_SignalConnect(parent,"hide",SDL_TooltipSignalHide,widget);

    return widget;
}

void SDL_TooltipDraw(SDL_Widget *widget,SDL_Window *dest,SDL_Rect *Area)
{ 
    SDL_Tooltip *Tooltip=(SDL_Tooltip*)widget;
    SDL_Rect r;

    r.x = widget->Rect.x ;
    r.y = widget->Rect.y;
    r.w = widget->Rect.w;
    r.h = widget->Rect.h;


    boxColor(dest->surface,r.x,r.y,r.x + r.w-1,r.y + r.h-1,0xf1ff89ff);
    rectangleColor(dest->surface,r.x,r.y,r.x + r.w-1,r.y + r.h-1,0x000000ff);
      
    if(Tooltip->Lines > 1)
    {
        char *str2;
        r.x = widget->Rect.x + 2;
        r.y = widget->Rect.y;
        r.w = widget->Rect.w;
        r.h = widget->Rect.h / 2;
        SDL_FontDrawStringWindow(dest,Tooltip->Font,&Tooltip->String,0x000000,&r);
        r.x = widget->Rect.x + 4;
        r.y = widget->Rect.y + r.h;
        r.w = widget->Rect.w;
        r.h = widget->Rect.h / 2;
        str2 = Tooltip->String.Data + strlen(Tooltip->String.Data)/2;
        SDL_FontDrawStringWindow(dest,Tooltip->Font,(SDL_String*)str2,0x000000,&r);

    }
    else
    {
        r.x = widget->Rect.x + 2;
        r.y = widget->Rect.y;
        r.w = widget->Rect.w;
        r.h = widget->Rect.h;
        SDL_FontDrawStringWindow(dest,Tooltip->Font,&Tooltip->String,0x000000,&r);
    }

}

Uint32 TimerCallback(Uint32 interval, void *param)
{
    SDL_Tooltip *Tooltip = (SDL_Tooltip*)param;
    SDL_Widget  *Widget  = (SDL_Widget*)param;
    SDL_Surface *VideoSurface;

    if(Widget->flags & WIDGET_VISIBLE)
    {
        Widget->Rect.x = Tooltip->x;
        Widget->Rect.y = Tooltip->y;

        Widget->Rect.h = SDL_FontGetHeight(Tooltip->Font) + 4;
        Widget->Rect.w = (Uint16)SDL_FontGetStringWidth(Tooltip->Font,Tooltip->String.Data) + 4;

        VideoSurface = SDLTk_GetVideoSurface();
        while(Widget->Rect.x + Widget->Rect.w > VideoSurface->clip_rect.x + VideoSurface->clip_rect.w)
        {
            Widget->Rect.h *= 2;
            Widget->Rect.w /= 2;
            Tooltip->Lines ++;
        }
        SDL_WidgetShow(Widget);
    }
    return 0;
}

static void SDL_TooltipSignalHide(SDL_Object *Object,void *signaldata,void *userdata)
{
    SDL_Widget *Widget = (SDL_Widget*)Object;
    SDL_Tooltip *Tooltip=(SDL_Tooltip*)Widget;

    if(Widget->flags & WIDGET_VISIBLE)
    {
        Tooltip->Lines=1;
        SDL_WidgetHide(Widget);
        SDL_RemoveTimer(Tooltip->Timer);
    }
}

static void SDL_TooltipSignalMouseMotion(SDL_Object *Object,void *signaldata,void *userdata)
{
    SDL_Tooltip *Tooltip = (SDL_Tooltip*)userdata;
    SDL_Event   *event   = (SDL_Event*)signaldata;

    if(SDL_WidgetIsInside(Tooltip->Parent,event->motion.x,event->motion.y))
    {
        Tooltip->x = event->motion.x + 10;
        Tooltip->y = event->motion.y + 10;

        SDL_RemoveTimer(Tooltip->Timer);
        Tooltip->Timer=SDL_AddTimer(1000,TimerCallback,Tooltip);
        
    }
    else
    {
        Tooltip->Lines=1;
        SDL_WidgetHide((SDL_Widget*)Tooltip);
        SDL_RemoveTimer(Tooltip->Timer);
    }
}

void SDL_TooltipSetFont(SDL_Widget *Widget,SDL_Font *Font)
{
    SDL_Tooltip *Tooltip=(SDL_Tooltip*)Widget;

    Tooltip->Font = Font;
}
