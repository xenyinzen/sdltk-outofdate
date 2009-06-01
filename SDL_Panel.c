/*
  SDLTk

  Implementation of Panel widget, can be fixed color or image
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

#include <SDL.h>
#include "SDLTk.h"
#include "SDL_Panel.h"

SDL_Surface *SDL_PanelPixelAccess(SDL_Widget *widget);
void SDL_PanelDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);
void SDL_PanelClose(SDL_Widget *widget);

SDL_WidgetType SDL_PanelGetType()
{
    SDL_WidgetType panel;
    
    panel=SDL_WidgetTypeGet("SDL_Panel");
    
    if(panel == 0)
    {
        SDL_WidgetFunctions *func;

        panel=SDL_WidgetTypeRegister("SDL_Panel",&func);
        func->draw  = SDL_PanelDraw;
        func->close = SDL_PanelClose;
    }

    return panel;
}



SDL_Widget* SDL_PanelNew()
{
    SDL_Widget *widget;
    SDL_Panel  *panel;
    
    panel=(SDL_Panel*) malloc(sizeof(SDL_Panel));
    memset(panel,0,sizeof(SDL_Panel));

    widget=(SDL_Widget*)panel;

    widget->Type      = SDL_PanelGetType();
    widget->Rect.x    = 0;
    widget->Rect.y    = 0;
    widget->Rect.w    = 0;
    widget->Rect.h    = 0;
    widget->flags     = 0;
    
    panel->color  = WHITE;
    panel->Image  = NULL;

	
    return widget;
}

void SDL_PanelDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Panel *Panel=(SDL_Panel*)widget;
    SDL_Rect src;
    SDL_Rect dst;

    /* Stop here if user has surface/pixel access */
    if(Panel->DisableDrawing)
        return;

    if(Panel->Image)
    {
        if(Area != NULL)
        {
            dst.x = Area->x;
            dst.y = Area->y;
            dst.w = Area->w;
            dst.h = Area->h;
            
            src.x = Area->x;
            src.y = Area->y;
            src.w = Area->w;
            src.h = Area->h;

            SDL_ImageBlitToSurface(Panel->Image,&src,surface,&dst);
        }
        else
        {
            dst.x = 0;
            dst.y = 0;

            if(Panel->Image->surface->w < widget->Rect.w)
                dst.w = Panel->Image->surface->w;
            
            dst.w = widget->Rect.w;
            dst.h = widget->Rect.h;

            SDL_ImageBlitToSurface(Panel->Image,NULL,surface,&dst);
        }
    }
    else
    {
        SDL_FillRect(surface,NULL,Panel->color);
    }
    
}


int SDL_PanelSetBackgroundColor(SDL_Widget *widget,Uint32 color)
{
    SDL_Panel *Panel=(SDL_Panel*)widget;
    Panel->color=color;
    SDL_WidgetRedrawEvent(widget);
    return 1;
}
static void SDL_PanelImageFrameUpdate(SDL_Object *object,void *signaldata,void *userdata)
{
	SDL_WidgetRedrawEvent(userdata);
}

int SDL_PanelSetImage(SDL_Widget *widget,SDL_Image *image)
{
    SDL_Panel *Panel=(SDL_Panel*)widget;

    if(widget==NULL || image== NULL)
    {
        printf("Error setting panel\n");
        return 0;
    }   

    if(Panel->Image == NULL)
    {
        Panel->Image = image;
		SDL_SignalObjectConnect((SDL_Object*)image,"frame-update",SDL_PanelImageFrameUpdate,widget);
        SDL_SetColorKey(Panel->Image->surface,SDL_SRCCOLORKEY,TRANSPARANT);
        
        if(widget->Rect.w == 0 && Panel->Image)
            widget->Rect.w=Panel->Image->surface->w;
        
        if(widget->Rect.h == 0 && Panel->Image)
            widget->Rect.h=Panel->Image->surface->h;

    }
    return 1;

}

SDL_Surface *SDL_PanelGetSurface(SDL_Widget *widget)
{
    SDL_Panel *Panel=(SDL_Panel*)widget;
    Panel->DisableDrawing = 1;

    SDL_FillRect(widget->Surface,NULL,0x0000ff);
    SDL_UpdateRect(widget->Surface,0,0,0,0);
    return widget->Surface;
}

void SDL_PanelClose(SDL_Widget *widget)
{
    SDL_Panel *Panel=(SDL_Panel*)widget;
    free(Panel);
}
